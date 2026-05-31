//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Type_Constraints_In_Templates_28
 */ 
class MutatorFrontendAction_28 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(28)

private:
    class MutatorASTConsumer_28 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_28(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/remove_type_constraints_in_templates_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      auto params = TD->getTemplateParameters();
      for (auto param : *params) {
        if (param->getKind() == Decl::Kind::TemplateTypeParm) {
          if (auto *tp = dyn_cast<TemplateTypeParmDecl>(param)) {
            if (tp->hasTypeConstraint()) {
              cur_templates.push_back(TD);
              auto original = stringutils::rangetoStr(
                  *(Result.SourceManager), tp->getSourceRange());
              auto modified = "typename " + tp->getNameAsString();
              llvm::outs() << original << "->" << modified << '\n';
              Rewrite.ReplaceText(tp->getSourceRange(), modified);
            }
          }
        }
      }
    } else if (auto *TU = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>(
                   "TemplateUsages")) {
      if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getBeginLoc()))
        return;
      auto decl = TU->getTemplateName().getAsTemplateDecl();
      for (auto target : cur_templates) {
        if (target == decl) {
          auto original = stringutils::rangetoStr(
              *(Result.SourceManager), TU->getSourceRange());
          auto modified = "/*mut28*/" + TU->desugar().getAsString();
          llvm::outs() << original << "->" << modified << '\n';
          Rewrite.ReplaceText(TU->getSourceRange(), modified);
        }
      }
    }
}
  
void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateDecl().bind("Templates");
    auto usage_matcher = templateSpecializationType().bind("TemplateUsages");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(usage_matcher, &callback);
    matchFinder.matchAST(Context);
}