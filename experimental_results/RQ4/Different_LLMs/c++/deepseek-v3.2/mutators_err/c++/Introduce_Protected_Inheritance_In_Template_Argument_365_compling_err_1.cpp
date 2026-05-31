//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Protected_Inheritance_In_Template_Argument_365
 */ 
class MutatorFrontendAction_365 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(365)

private:
    class MutatorASTConsumer_365 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_365(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> candidateClasses;
    };
};

//source file
#include "../include/Mutator_Introduce_Protected_Inheritance_In_Template_Argument_365.h"

// ========================================================================================================
#define MUT365_OUTPUT 1

void MutatorFrontendAction_365::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
        return;
      if (!CRD->isCompleteDefinition())
        return;
      //Record candidate classes that are used as template arguments
      candidateClasses.push_back(CRD);
    }
    else if (auto *TTP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParm")) {
      //Filter nodes in header files
      if (!TTP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TTP->getLocation()))
        return;
      //Get the source code text of target node
      auto templateName = TTP->getNameAsString();
      if (templateName.empty())
        return;
      
      //Find a candidate class that matches the template parameter type
      const clang::CXXRecordDecl *targetClass = nullptr;
      for (const auto *CRD : candidateClasses) {
        if (CRD->getNameAsString() == templateName) {
          targetClass = CRD;
          break;
        }
      }
      if (!targetClass)
        return;
      
      //Check if the class has at least one base class
      if (!targetClass->hasDefinition() || targetClass->getNumBases() == 0)
        return;
      
      //Select a random base class to modify inheritance
      int baseIndex = getrandom::getRandomIndex(targetClass->getNumBases() - 1);
      auto baseSpec = targetClass->getBases()[baseIndex];
      auto baseType = baseSpec.getType();
      const clang::CXXRecordDecl *baseClass = baseType->getAsCXXRecordDecl();
      if (!baseClass)
        return;
      
      //Get the source range of the base specifier
      SourceRange baseRange = baseSpec.getSourceRange();
      if (baseRange.isInvalid())
        return;
      
      //Get the original inheritance specifier text
      std::string originalInheritance = stringutils::rangetoStr(*(Result.SourceManager), baseRange);
      
      //Perform mutation on the source code text by applying string replacement
      //Replace public inheritance with protected inheritance
      std::string mutatedInheritance;
      if (originalInheritance.find("public") != std::string::npos) {
        mutatedInheritance = originalInheritance;
        size_t pos = mutatedInheritance.find("public");
        mutatedInheritance.replace(pos, 6, "protected");
      } else if (originalInheritance.find("private") != std::string::npos) {
        mutatedInheritance = originalInheritance;
        size_t pos = mutatedInheritance.find("private");
        mutatedInheritance.replace(pos, 7, "protected");
      } else {
        //If no explicit specifier (struct default public), insert protected
        mutatedInheritance = "protected " + originalInheritance;
      }
      
      mutatedInheritance = "/*mut365*/" + mutatedInheritance;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(baseRange, mutatedInheritance);
    }
}
  
void MutatorFrontendAction_365::MutatorASTConsumer_365::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("ClassDecl");
    DeclarationMatcher templateMatcher = templateTypeParmDecl().bind("TemplateTypeParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}