//source file
#include "../include/modify_ternary_array_types_498.h"

// ========================================================================================================
#define MUT498_OUTPUT 1

void MutatorFrontendAction_498::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::ConditionalOperator>("condOp")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
        return;

      const Expr *cond = CE->getCond();
      const Expr *trueExpr = CE->getTrueExpr();
      const Expr *falseExpr = CE->getFalseExpr();

      if (cond->getType()->isBooleanType() && trueExpr->getType()->isArrayType() && falseExpr->getType()->isArrayType()) {
        const auto *trueArrayType = trueExpr->getType()->getAsArrayTypeUnsafe();
        const auto *falseArrayType = falseExpr->getType()->getAsArrayTypeUnsafe();

        if (trueArrayType && falseArrayType && trueArrayType != falseArrayType) {
          std::string trueExprStr = Lexer::getSourceText(CharSourceRange::getTokenRange(trueExpr->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
          std::string falseExprStr = Lexer::getSourceText(CharSourceRange::getTokenRange(falseExpr->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();

          std::string newTrueExpr = "/*mut498*/" + trueExprStr + "[3]"; // Example modification
          Rewrite.ReplaceText(trueExpr->getSourceRange(), newTrueExpr);
        }
      }
    }
}
  
void MutatorFrontendAction_498::MutatorASTConsumer_498::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conditionalOperator(hasCondition(expr().bind("cond")), 
                                       hasTrueExpression(expr().bind("trueExpr")), 
                                       hasFalseExpression(expr().bind("falseExpr"))).bind("condOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}