//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Ternary_Expr1_With_Throw_399
 */ 
class MutatorFrontendAction_399 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(399)

private:
    class MutatorASTConsumer_399 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_399(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/replace_ternary_expr1_with_throw_399.h"

// ========================================================================================================
#define MUT399_OUTPUT 1

void MutatorFrontendAction_399::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::ConditionalOperator>("TernaryOp")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocation()))
        return;

      auto *Expr1 = CE->getTrueExpr();
      if (!Expr1)
        return;

      auto ThrowExpr = "throw 0";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Expr1->getSourceRange()), ThrowExpr);
    }
}
  
void MutatorFrontendAction_399::MutatorASTConsumer_399::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conditionalOperator().bind("TernaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}