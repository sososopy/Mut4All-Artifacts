//header file
#pragma once
#include "Mutator_base.h"

/**
 * Partial_Specialization_Default_Argument_Substitution_407
 */ 
class MutatorFrontendAction_407 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(407)

private:
    class MutatorASTConsumer_407 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_407(Rewriter &R) : TheRewriter(R) {}
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
        const clang::ClassTemplateDecl *primary = nullptr;
    };
};

//source file
#include "../include/Partial_Specialization_Default_Argument_Substitution_407.h"

// ========================================================================================================
#define MUT407_OUTPUT 1

void MutatorFrontendAction_407::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PS = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpecialization")) {
      if (!PS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PS->getLocation()))
        return;
      if (!PS->isCompleteDefinition())
        return;
      if (primary == nullptr)
        return;
      auto primary_params = primary->getTemplateParameters();
      auto partial_params = PS->getTemplateParameters();
      if (primary_params->size() > partial_params->size())
        return;
      auto partial_content = stringutils::rangetoStr(
          *(Result.SourceManager), PS->getSourceRange());
      for (std::size_t i = 0; i < primary_params->size(); ++i) {
        auto primary_param = primary_params->getParam(i);
        auto partial_param = partial_params->getParam(i);
        if (auto primary_tparam = dyn_cast<TemplateTypeParmDecl>(primary_param)) {
          if (auto partial_tparam =
                  dyn_cast<TemplateTypeParmDecl>(partial_param)) {
            if (partial_tparam->hasDefaultArgument()) {
              auto default_arg = partial_tparam->getDefaultArgument();
              if (default_arg.getAsType().getAsString() ==
                  primary_tparam->getNameAsString()) {
                auto replace_name = partial_tparam->getNameAsString();
                auto replace_content = partial_tparam->getNameAsString() +
                                       "=" + replace_name;
                llvm::outs() << replace_content << '\n';
                llvm::outs() << partial_content << '\n';
                llvm::outs() << partial_content.find(replace_content) << '\n';
                partial_content.replace(partial_content.find(replace_content),
                                        replace_content.length(), replace_name);
              }
            }
          }
        }
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(PS->getSourceRange()), partial_content);
    } else if (auto *PT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "PrimaryTemplate")) {
      if (!PT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PT->getLocation()))
        return;
      if (!PT->isThisDeclarationADefinition())
        return;
      primary = PT;
    }
}
  
void MutatorFrontendAction_407::MutatorASTConsumer_407::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto partial_specialization_matcher =
        classTemplatePartialSpecializationDecl().bind("PartialSpecialization");
    auto primary_template_matcher = classTemplateDecl().bind("PrimaryTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(partial_specialization_matcher, &callback);
    matchFinder.addMatcher(primary_template_matcher, &callback);
    matchFinder.matchAST(Context);
}