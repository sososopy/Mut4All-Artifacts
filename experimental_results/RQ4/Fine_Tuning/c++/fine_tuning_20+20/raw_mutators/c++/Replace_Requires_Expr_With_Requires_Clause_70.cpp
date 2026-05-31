//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_Expr_With_Requires_Clause_70
 */ 
class MutatorFrontendAction_70 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(70)

private:
    class MutatorASTConsumer_70 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_70(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::RequiresExpr *> requires_exprs;
    };
};

//source file
#include "../include/replace_requires_expr_with_requires_clause_70.h"

// ========================================================================================================
#define MUT70_OUTPUT 1

void MutatorFrontendAction_70::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RE = Result.Nodes.getNodeAs<clang::RequiresExpr>("requiresExpr")) {
      if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RE->getBeginLoc()))
        return;
      requires_exprs.push_back(RE);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::Decl>("decl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (requires_exprs.empty())
        return;
      auto target = requires_exprs.back();
      requires_exprs.pop_back();
      auto requires_content = stringutils::rangetoStr(
          *(Result.SourceManager), target->getLocalRange());
      llvm::outs() << requires_content << '\n';
      Rewrite.ReplaceText(target->getSourceRange(), requires_content);
      Rewrite.ReplaceText(DL->getBeginLoc(), 0, "/*mut70*/requires " + requires_content + '\n');
    }
}
  
void MutatorFrontendAction_70::MutatorASTConsumer_70::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = requiresExpr().bind("requiresExpr");
    auto decl_matcher = decl().bind("decl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}