//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inconsistent_Template_Specialization_Inheritance_Mismatch_235
 */ 
class MutatorFrontendAction_235 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(235)

private:
    class MutatorASTConsumer_235 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_235(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> templateDerivedClasses;
        std::vector<const clang::CXXRecordDecl *> templateBaseClasses;
    };
};

//source file
#include "../include/Mutator_Inconsistent_Template_Specialization_Inheritance_Mismatch_235.h"

// ========================================================================================================
#define MUT235_OUTPUT 1

void MutatorFrontendAction_235::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateDerived")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if (!TD->isTemplateDecl() || !TD->isCompleteDefinition())
        //Record the node information to be used in the mutation process
        templateDerivedClasses.push_back(TD);
    }
    else if (auto *TB = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateBase")) {
      //Filter nodes in header files
      if (!TB || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TB->getLocation()))
        return;
      if (!TB->isTemplateDecl() || !TB->isCompleteDefinition())
        //Record the node information to be used in the mutation process
        templateBaseClasses.push_back(TB);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
        auto param = FD->getParamDecl(i);
        auto paramType = param->getType();
        if (paramType->isTemplateTypeParmType()) {
          auto templateType = paramType->getAs<clang::TemplateTypeParmType>();
          if (!templateType) continue;
          auto templateDecl = templateType->getDecl();
          if (!templateDecl) continue;
          auto templateRecord = dyn_cast<CXXRecordDecl>(templateDecl);
          if (!templateRecord) continue;
          
          // Check if this templateRecord is in our derived classes list
          bool isDerived = false;
          for (auto derivedClass : templateDerivedClasses) {
            if (derivedClass == templateRecord) {
              isDerived = true;
              break;
            }
          }
          if (!isDerived) continue;
          
          // Find the base class for this derived template class
          const CXXRecordDecl *baseClass = nullptr;
          for (auto baseClassCandidate : templateBaseClasses) {
            // Check if derivedClass inherits from baseClassCandidate
            for (auto base : templateRecord->bases()) {
              auto baseType = base.getType();
              if (baseType->isTemplateTypeParmType()) {
                auto baseTemplateType = baseType->getAs<clang::TemplateTypeParmType>();
                if (!baseTemplateType) continue;
                auto baseTemplateDecl = baseTemplateType->getDecl();
                if (!baseTemplateDecl) continue;
                auto baseTemplateRecord = dyn_cast<CXXRecordDecl>(baseTemplateDecl);
                if (baseTemplateRecord == baseClassCandidate) {
                  baseClass = baseClassCandidate;
                  break;
                }
              }
            }
            if (baseClass) break;
          }
          if (!baseClass) continue;
          
          // Get the template argument from the parameter type
          auto templateDecl = templateRecord->getDescribedClassTemplate();
          if (!templateDecl) continue;
          auto templateParams = templateDecl->getTemplateParameters();
          if (templateParams->size() == 0) continue;
          std::string argStr = templateParams->getParam(0)->getNameAsString();
          
          //Perform mutation on the1 source code text by applying string replacement
          int choice = getrandom::getRandomIndex(1);
          std::string mutatedArgStr;
          if (choice == 0) {
            mutatedArgStr = "const " + argStr;
          } else {
            mutatedArgStr = "volatile " + argStr;
          }
          
          // Build the new parameter type string
          std::string newParamType = templateRecord->getNameAsString() + "<" + mutatedArgStr + ">";
          
          //Replace the original AST node with the mutated one
          SourceRange paramRange = param->getSourceRange();
          Rewrite.ReplaceText(paramRange, newParamType);
        }
      }
    }
}
  
void MutatorFrontendAction_235::MutatorASTConsumer_235::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl(isTemplateInstantiation()))).bind("TemplateDerived");
    DeclarationMatcher baseMatcher = cxxRecordDecl(isTemplateInstantiation()).bind("TemplateBase");
    DeclarationMatcher functionMatcher = functionDecl(hasAnyParameter(hasType(templateTypeParmType()))).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}