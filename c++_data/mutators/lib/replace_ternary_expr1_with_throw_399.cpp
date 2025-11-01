//source file
#include "../include/replace_ternary_expr1_with_throw_399.h"

// ========================================================================================================
#define MUT399_OUTPUT 1

void MutatorFrontendAction_399::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::ConditionalOperator>("TernaryOp")) {
      if (!CE || !Result.SourceManager->isWrittenInMainFile(
                     CE->getExprLoc()))
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