//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nest_Namespace_With_Inline_Function_194
 */ 
class MutatorFrontendAction_194 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(194)

private:
    class MutatorASTConsumer_194 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_194(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::NamespaceDecl*> visitedNamespaces;
    };
};

//source file
#include "../include/Mutator_Nest_Namespace_With_Inline_Function_194.h"

// ========================================================================================================
#define MUT194_OUTPUT 1

void MutatorFrontendAction_194::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NamespaceDecl")) {
      //Filter nodes in header files
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      // Avoid processing the same namespace multiple times
      if (std::find(visitedNamespaces.begin(), visitedNamespaces.end(), NS) != visitedNamespaces.end())
        return;
      visitedNamespaces.push_back(NS);

      //Get the source code text of target node
      SourceManager &SM = *Result.SourceManager;
      LangOptions LangOpts;
      std::string originalNamespaceStr;
      llvm::raw_string_ostream OS(originalNamespaceStr);
      NS->print(OS, PrintingPolicy(LangOpts));
      OS.flush();

      // Find the first class/struct inside this namespace
      const clang::CXXRecordDecl* targetClass = nullptr;
      clang::CXXMethodDecl* methodToMove = nullptr;
      for (auto it = NS->decls_begin(); it != NS->decls_end(); ++it) {
        if (auto *RD = llvm::dyn_cast<clang::CXXRecordDecl>(*it)) {
          if (RD->isCompleteDefinition()) {
            targetClass = RD;
            // Find first non-constructor, non-destructor method with a body
            for (auto method : RD->methods()) {
              if (method->hasBody() && !llvm::isa<clang::CXXConstructorDecl>(method) && 
                  !llvm::isa<clang::CXXDestructorDecl>(method)) {
                methodToMove = method;
                break;
              }
            }
            break;
          }
        }
      }

      std::string mutation;
      std::string innerNamespaceName = NS->getNameAsString() + "_inner";
      
      if (targetClass && methodToMove) {
        // Case 1: Move existing method to inner namespace
        // Get method source
        std::string methodSource;
        llvm::raw_string_ostream methodOS(methodSource);
        methodToMove->print(methodOS, PrintingPolicy(LangOpts));
        methodOS.flush();
        
        // Create inner namespace with moved method
        mutation = "namespace " + innerNamespaceName + " {\n";
        mutation += methodSource + "\n";
        mutation += "}\n";
        
        // Remove method from class (replace with declaration)
        std::string classSource;
        llvm::raw_string_ostream classOS(classSource);
        targetClass->print(classOS, PrintingPolicy(LangOpts));
        classOS.flush();
        
        // Find and remove the method definition from class
        size_t methodPos = classSource.find(methodSource);
        if (methodPos != std::string::npos) {
          // Replace with just declaration
          std::string declOnly = methodToMove->getReturnType().getAsString() + " " + 
                                methodToMove->getNameAsString() + "(";
          // Add parameters
          for (unsigned i = 0; i < methodToMove->getNumParams(); ++i) {
            if (i > 0) declOnly += ", ";
            declOnly += methodToMove->getParamDecl(i)->getType().getAsString() + 
                       " " + methodToMove->getParamDecl(i)->getNameAsString();
          }
          declOnly += ");";
          
          classSource.replace(methodPos, methodSource.length(), declOnly);
          
          //Perform mutation on the source code text by applying string replacement
          // First insert inner namespace at beginning of original namespace
          SourceLocation insertLoc = NS->getBeginLoc().getLocWithOffset(1);
          Rewrite.InsertTextAfterToken(insertLoc, "\nnamespace " + innerNamespaceName + " {\n");
          
          // Insert method definition in inner namespace
          SourceLocation innerInsert = insertLoc.getLocWithOffset(innerNamespaceName.length() + 12);
          std::string movedMethod = methodToMove->getReturnType().getAsString() + " " + 
                                   targetClass->getNameAsString() + "::" + 
                                   methodToMove->getNameAsString() + "(";
          for (unsigned i = 0; i < methodToMove->getNumParams(); ++i) {
            if (i > 0) movedMethod += ", ";
            movedMethod += methodToMove->getParamDecl(i)->getType().getAsString() + 
                          " " + methodToMove->getParamDecl(i)->getNameAsString();
          }
          movedMethod += ") { return 0; }";
          Rewrite.InsertTextAfterToken(innerInsert, "\n" + movedMethod + "\n");
          
          // Close inner namespace
          SourceLocation beforeClass = targetClass->getBeginLoc();
          Rewrite.InsertTextBefore(beforeClass, "}\n");
          
          // Replace class with modified version (without method body)
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(targetClass->getSourceRange()), classSource);
        }
      } else {
        // Case 2: No suitable class/method found, create simple struct with method
        mutation = "namespace " + innerNamespaceName + " {\n";
        mutation += "struct Mut_194_S { int value; };\n";
        mutation += "int Mut_194_S_getValue() { return 0; }\n";
        mutation += "}\n";
        
        //Perform mutation on the source code text by applying string replacement
        SourceLocation insertLoc = NS->getBeginLoc().getLocWithOffset(1);
        Rewrite.InsertTextAfterToken(insertLoc, "\n" + mutation);
      }
    }
}
  
void MutatorFrontendAction_194::MutatorASTConsumer_194::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl().bind("NamespaceDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}