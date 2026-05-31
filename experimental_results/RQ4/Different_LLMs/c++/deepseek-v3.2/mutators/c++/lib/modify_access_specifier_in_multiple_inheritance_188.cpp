//source file
#include "../include/Mutator_Modify_Access_Specifier_In_Multiple_Inheritance_188.h"

// ========================================================================================================
#define MUT188_OUTPUT 1

void MutatorFrontendAction_188::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseDecl")) {
      //Filter nodes in header files
      if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BD->getLocation()))
        return;
      if ((!BD->isStruct() && !BD->isClass()) || BD->isLambda())
        return;
      if (!BD->isCompleteDefinition())
        return;
      //Record base class for later analysis
      baseClasses.push_back(BD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record candidate fields with public or protected access
      if (FD->getAccess() == AccessSpecifier::AS_public || 
          FD->getAccess() == AccessSpecifier::AS_protected) {
        candidateFields.push_back(FD);
      }
    }
    else if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodDecl")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      //Record candidate methods with public or protected access
      if (MD->getAccess() == AccessSpecifier::AS_public || 
          MD->getAccess() == AccessSpecifier::AS_protected) {
        candidateMethods.push_back(MD);
      }
    }
    else if (auto *DD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedDecl")) {
      //Filter nodes in header files
      if (!DD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DD->getLocation()))
        return;
      if ((!DD->isStruct() && !DD->isClass()) || DD->isLambda())
        return;
      if (!DD->isCompleteDefinition())
        return;
      
      //Check if this is a multiple inheritance scenario
      int baseCount = 0;
      for (auto base : DD->bases()) {
        baseCount++;
        if (baseCount >= 2) break;
      }
      if (baseCount < 2) return;
      
      //Find a base class that's part of this inheritance chain
      const CXXRecordDecl* targetBase = nullptr;
      for (auto baseClass : baseClasses) {
        if (DD->isDerivedFrom(baseClass)) {
          targetBase = baseClass;
          break;
        }
      }
      
      if (!targetBase) return;
      
      //Try to modify existing member access first
      bool mutationApplied = false;
      
      //Try fields
      for (auto field : candidateFields) {
        if (field->getParent() == targetBase) {
          //Get the source code text of target node
          auto fieldRange = field->getSourceRange();
          auto fieldText = stringutils::rangetoStr(*(Result.SourceManager), fieldRange);
          
          //Perform mutation on the source code text by applying string replacement
          //Change public/protected to private
          size_t accessPos = fieldText.find("public:");
          if (accessPos != std::string::npos) {
            fieldText.replace(accessPos, 6, "private:");
          } else {
            accessPos = fieldText.find("protected:");
            if (accessPos != std::string::npos) {
              fieldText.replace(accessPos, 9, "private:");
            }
          }
          
          if (accessPos != std::string::npos) {
            fieldText = "/*mut188*/" + fieldText;
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(fieldRange, fieldText);
            mutationApplied = true;
            break;
          }
        }
      }
      
      //If no field mutation, try methods
      if (!mutationApplied) {
        for (auto method : candidateMethods) {
          if (method->getParent() == targetBase) {
            //Get the source code text of target node
            auto methodRange = method->getSourceRange();
            auto methodText = stringutils::rangetoStr(*(Result.SourceManager), methodRange);
            
            //Perform mutation on the source code text by applying string replacement
            size_t accessPos = methodText.find("public:");
            if (accessPos != std::string::npos) {
              methodText.replace(accessPos, 6, "private:");
            } else {
              accessPos = methodText.find("protected:");
              if (accessPos != std::string::npos) {
                methodText.replace(accessPos, 9, "private:");
              }
            }
            
            if (accessPos != std::string::npos) {
              methodText = "/*mut188*/" + methodText;
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(methodRange, methodText);
              mutationApplied = true;
              break;
            }
          }
        }
      }
      
      //If still no mutation, add a new private member
      if (!mutationApplied) {
        //Get the source code text of target node
        auto baseRange = targetBase->getSourceRange();
        auto baseText = stringutils::rangetoStr(*(Result.SourceManager), baseRange);
        
        //Perform mutation on the source code text by applying string replacement
        //Insert private static member before the closing brace
        size_t lastBrace = baseText.rfind('}');
        if (lastBrace != std::string::npos) {
          std::string newMember = "\n\t/*mut188*/private:\n\tstatic int mutated_member_188;\n";
          baseText.insert(lastBrace, newMember);
          
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(baseRange, baseText);
        }
      }
    }
}
  
void MutatorFrontendAction_188::MutatorASTConsumer_188::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseDecl");
    DeclarationMatcher fieldMatcher = fieldDecl().bind("FieldDecl");
    DeclarationMatcher methodMatcher = cxxMethodDecl().bind("MethodDecl");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}