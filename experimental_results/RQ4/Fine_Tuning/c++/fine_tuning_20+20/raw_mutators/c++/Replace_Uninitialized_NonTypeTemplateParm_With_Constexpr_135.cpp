//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Uninitialized_NonTypeTemplateParm_With_Constexpr_135
 */ 
class MutatorFrontendAction_135 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(135)

private:
    class MutatorASTConsumer_135 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_135(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> target_templates;
    };
};

//source file
#include "../include/replace_uninitialized_nontype_templateparm_with_constexpr_135.h"

// ========================================================================================================
#define MUT135_OUTPUT 1

void MutatorFrontendAction_135::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      if (!TP->isThisDeclarationADefinition())
        return;
      auto params = TP->getTemplateParameters();
      if (params->size() == 0)
        return;
      auto target_decl = TP->getTemplatedDecl();
      if (!target_decl->isCompleteDefinition())
        return;
      if (target_decl->isLambda())
        return;
      if (!target_decl->hasDefinition())
        return;
      for (auto param : *params) {
        if (auto NTTP = dyn_cast<NonTypeTemplateParmDecl>(param)) {
          if (NTTP->hasDefaultArgument())
            continue;
          if (NTTP->hasTypeConstraint() == false)
            continue;
          target_templates.push_back(TP);
          break;
        }
      }
    } else if (auto *VD =
                   Result.Nodes.getNodeAs<clang::VarDecl>("UninitVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->hasInit())
        return;
      if (VD->getStorageDuration() != StorageDuration::SD_Automatic)
        return;
      auto type = VD->getType();
      auto type_str = type.getAsString();
      auto name = VD->getNameAsString();
      auto replacement = "/*mut135*/constexpr " + type_str + " " + name;
      Rewrite.ReplaceText(VD->getBeginLoc(), replacement);
    }
}
  
void MutatorFrontendAction_135::MutatorASTConsumer_135::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto var_matcher = varDecl().bind("UninitVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}