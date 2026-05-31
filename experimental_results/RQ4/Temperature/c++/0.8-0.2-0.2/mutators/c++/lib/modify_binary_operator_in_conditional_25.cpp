//source file
#include "../include/modify_binary_operator_in_conditional_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BO->getExprLoc()))
            return;

        const Expr *lhs = BO->getLHS();
        const Expr *rhs = BO->getRHS();
        QualType lhsType = lhs->getType();
        QualType rhsType = rhs->getType();

        if (lhsType->isIntegerType() && !rhsType->isIntegerType()) {
            std::string newExpr = "(" + std::string(lhs->getStmtClassName()) + " < reinterpret_cast<int>(" + std::string(rhs->getStmtClassName()) + "))";
            Rewrite.ReplaceText(BO->getSourceRange(), newExpr);
        } else if (!lhsType->isIntegerType() && rhsType->isIntegerType()) {
            std::string newExpr = "(reinterpret_cast<int>(" + std::string(lhs->getStmtClassName()) + ") < " + std::string(rhs->getStmtClassName()) + ")";
            Rewrite.ReplaceText(BO->getSourceRange(), newExpr);
        }
    }
}
  
void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = binaryOperator(hasOperatorName("<")).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}