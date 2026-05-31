//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Pack_With_Empty_Pack_204
 */ 
class MutatorFrontendAction_204 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(204)

private:
    class MutatorASTConsumer_204 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_204(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateSpecializationDecl*> specializations;
    };
};

//source file
#include "../include/Replace_Template_Pack_With_Empty_Pack_204.h"

// ========================================================================================================
#define MUT204_OUTPUT 1

void MutatorFrontendAction_204::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specialization")) {
      //Filter nodes in header files
      if (!TSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TSD->getLocation()))
        return;
      
      // Check if template has parameter pack
      auto *Template = TSD->getSpecializedTemplate();
      if (!Template) return;
      
      auto *TemplateParams = Template->getTemplateParameters();
      if (!TemplateParams) return;
      
      bool hasPack = false;
      for (unsigned i = 0; i < TemplateParams->size(); ++i) {
        if (auto *Param = dyn_cast<TemplateTypeParmDecl>(TemplateParams->getParam(i))) {
          if (Param->isParameterPack()) {
            hasPack = true;
            break;
          }
        } else if (auto *Param = dyn_cast<NonTypeTemplateParmDecl>(TemplateParams->getParam(i))) {
          if (Param->isParameterPack()) {
            hasPack = true;
            break;
          }
        } else if (auto *Param = dyn_cast<TemplateTemplateParmDecl>(TemplateParams->getParam(i))) {
          if (Param->isParameterPack()) {
            hasPack = true;
            break;
          }
        }
      }
      
      if (!hasPack) return;
      
      // Check if instantiation has at least one argument
      const clang::TemplateArgumentList &Args = TSD->getTemplateArgs();
      if (Args.size() == 0) return;
      
      // Record the specialization
      specializations.push_back(TSD);
    }
    else if (auto *TSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("SelectedSpecialization")) {
      //Filter nodes in header files
      if (!TSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TSD->getLocation()))
        return;
      
      // Select one specialization to mutate
      if (specializations.empty()) return;
      
      size_t index = getrandom::getRandomIndex(specializations.size() - 1);
      auto *TargetSpec = specializations[index];
      
      if (TargetSpec != TSD) return;
      
      //Get the source code text of target node
      auto sourceRange = TargetSpec->getSourceRange();
      if (!sourceRange.isValid()) return;
      
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      // Find the template name and replace arguments with empty pack
      std::string templateName = TargetSpec->getSpecializedTemplate()->getNameAsString();
      
      // Find the position of template arguments
      size_t templateStart = originalText.find(templateName);
      if (templateStart == std::string::npos) return;
      
      size_t argsStart = originalText.find('<', templateStart);
      if (argsStart == std::string::npos) return;
      
      size_t argsEnd = originalText.find('>', argsStart);
      if (argsEnd == std::string::npos) return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = originalText;
      mutatedText.replace(argsStart + 1, argsEnd - argsStart - 1, "");
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_204::MutatorASTConsumer_204::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("Specialization");
    DeclarationMatcher selectedMatcher = classTemplateSpecializationDecl().bind("SelectedSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(selectedMatcher, &callback);
    matchFinder.matchAST(Context);
}