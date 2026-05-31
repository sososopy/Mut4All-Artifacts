//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Default_Template_Argument_In_Partial_Specialization_149
 */ 
class MutatorFrontendAction_149 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(149)

private:
    class MutatorASTConsumer_149 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_149(Rewriter &R) : TheRewriter(R) {}
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
        const clang::ClassTemplateDecl *cur_template = nullptr;
    };
};

//source file
#include "../include/modify_default_template_argument_in_partial_specialization_149.h"

// ========================================================================================================
#define MUT149_OUTPUT 1

void MutatorFrontendAction_149::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      llvm::outs() << MT->getNameAsString() << '\n';
      cur_template = MT;
    } else if (auto *PS = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>(
                   "PartialSpecialization")) {
      if (!PS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PS->getLocation()))
        return;
      if (cur_template == nullptr)
        return;
      if (PS->getSpecializedTemplate() != cur_template)
        return;
      auto params = PS->getTemplateParameters();
      std::vector<int> default_params_index;
      for (size_t i = 0; i < params->size(); ++i) {
        if (params->getParam(i)->hasDefaultArgument())
          default_params_index.push_back(i);
      }
      if (default_params_index.empty())
        return;
      int choice = getrandom::getRandomIndex(default_params_index.size() - 1);
      int target_index = default_params_index[choice];
      auto target_param = params->getParam(target_index);
      auto target_default = target_param->getDefaultArgument();
      auto target_default_str =
          stringutils::rangetoStr(*(Result.SourceManager),
                                  target_default->getSourceRange());
      llvm::outs() << target_default_str << '\n';
      auto template_params = cur_template->getTemplateParameters();
      std::vector<int> non_default_params_index;
      for (size_t i = 0; i < template_params->size(); ++i) {
        if (!template_params->getParam(i)->hasDefaultArgument())
          non_default_params_index.push_back(i);
      }
      if (non_default_params_index.empty()) {
        if (target_default_str == "int")
          target_default_str = "double";
        else
          target_default_str = "int";
      } else {
        int choice2 =
            getrandom::getRandomIndex(non_default_params_index.size() - 1);
        int target_index2 = non_default_params_index[choice2];
        auto target_param2 = template_params->getParam(target_index2);
        target_default_str = target_param2->getNameAsString();
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(target_default->getSourceRange()),
          "/*mut149*/" + target_default_str);
    }
}
  
void MutatorFrontendAction_149::MutatorASTConsumer_149::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto partial_specialization_matcher =
        classTemplatePartialSpecializationDecl().bind("PartialSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(partial_specialization_matcher, &callback);
    matchFinder.matchAST(Context);
}