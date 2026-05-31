//source file
#include "../include/Insert_Consteval_Constructor_In_Loop_76.h"

// ========================================================================================================
#define MUT76_OUTPUT 1

void MutatorFrontendAction_76::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      //Record the node information to be used in the mutation process
      candidateClasses.push_back(CL);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->hasBody())
        return;
      //Record the node information to be used in the mutation process
      candidateMethods.push_back(MT);
    }
    else if (auto *FS = Result.Nodes.getNodeAs<clang::ForStmt>("ForStmt")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getBeginLoc()))
        return;

      //Select a candidate class and method
      if (candidateClasses.empty() || candidateMethods.empty())
        return;
      size_t classIdx = getrandom::getRandomIndex(candidateClasses.size() - 1);
      size_t methodIdx = getrandom::getRandomIndex(candidateMethods.size() - 1);
      const CXXRecordDecl *targetClass = candidateClasses[classIdx];
      const CXXMethodDecl *targetMethod = candidateMethods[methodIdx];

      //Check if the method belongs to the class
      if (targetMethod->getParent() != targetClass)
        return;

      //Determine if the class already has a consteval constructor
      bool hasConstevalConstructor = false;
      for (auto *ctor : targetClass->ctors()) {
        if (ctor->isConsteval()) {
          hasConstevalConstructor = true;
          break;
        }
      }

      //Prepare insertion text
      std::string insertion;
      std::string aliasDef = "";
      std::string aliasUse = "";

      //If no consteval constructor exists, add one
      if (!hasConstevalConstructor) {
        //Add a templated consteval constructor before the method
        SourceLocation classEnd = targetClass->getEndLoc();
        std::string ctorDecl = "\n\tpublic:\n\ttemplate<typename T>\n\tconsteval " + targetClass->getNameAsString() + "(T param) {}";
        Rewrite.InsertTextBefore(classEnd, ctorDecl);
        //Define an alias template
        aliasDef = "\n\ttemplate<typename... Args>\n\tusing Alias_" + targetClass->getNameAsString() + " = " + targetClass->getNameAsString() + ";";
        Rewrite.InsertTextBefore(classEnd, aliasDef);
        aliasUse = "Alias_" + targetClass->getNameAsString() + "<> instance(param);";
      } else {
        //Use existing consteval constructor, find one
        for (auto *ctor : targetClass->ctors()) {
          if (ctor->isConsteval()) {
            //Check if there's an alias template already
            bool aliasExists = false;
            for (auto *decl : targetClass->decls()) {
              if (auto *alias = dyn_cast<TypeAliasDecl>(decl)) {
                if (alias->getUnderlyingType().getAsString() == targetClass->getNameAsString()) {
                  aliasExists = true;
                  aliasUse = alias->getNameAsString() + "<> instance(param);";
                  break;
                }
              }
            }
            if (!aliasExists) {
              //Add alias definition
              aliasDef = "\n\ttemplate<typename... Args>\n\tusing Alias_" + targetClass->getNameAsString() + " = " + targetClass->getNameAsString() + ";";
              SourceLocation classEnd = targetClass->getEndLoc();
              Rewrite.InsertTextBefore(classEnd, aliasDef);
              aliasUse = "Alias_" + targetClass->getNameAsString() + "<> instance(param);";
            }
            break;
          }
        }
      }

      //Determine parameter for constructor call
      std::string param = "0"; // default
      if (FS->getInit()) {
        //Try to get a variable from the for-loop init
        if (auto *declStmt = dyn_cast<DeclStmt>(FS->getInit())) {
          for (auto *decl : declStmt->decls()) {
            if (auto *varDecl = dyn_cast<VarDecl>(decl)) {
              param = varDecl->getNameAsString();
              break;
            }
          }
        }
      }

      //Construct the insertion text
      if (aliasUse.empty()) {
        insertion = "\n\t\t" + targetClass->getNameAsString() + " instance(" + param + ");";
      } else {
        insertion = "\n\t\t" + aliasUse;
      }

      //Perform mutation on the source code text by applying string replacement
      Rewrite.InsertTextAfterToken(FS->getBody()->getBeginLoc(), insertion);
    }
}
  
void MutatorFrontendAction_76::MutatorASTConsumer_76::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("Classes");
    DeclarationMatcher methodMatcher = cxxMethodDecl().bind("Methods");
    StatementMatcher forMatcher = forStmt().bind("ForStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.addMatcher(forMatcher, &callback);
    matchFinder.matchAST(Context);
}