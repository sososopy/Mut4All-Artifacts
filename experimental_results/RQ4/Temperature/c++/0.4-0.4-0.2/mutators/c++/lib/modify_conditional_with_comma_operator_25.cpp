//source file
#include "../include/modify_conditional_with_comma_operator_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *IfStmt = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
        if (!IfStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                       IfStmt->getIfLoc()))
            return;

        auto *Cond = IfStmt->getCond();
        if (!Cond)
            return;

        // Check if the condition is a binary operator
        if (auto *BinOp = llvm::dyn_cast<clang::BinaryOperator>(Cond)) {
            if (BinOp->isAssignmentOp()) {
                auto LHS = BinOp->getLHS()->IgnoreImpCasts();
                auto RHS = BinOp->getRHS()->IgnoreImpCasts();

                // Create a new condition with a comma operator
                std::string newCond = "(" + Rewrite.getRewrittenText(LHS->getSourceRange()) +
                                      " = " + Rewrite.getRewrittenText(RHS->getSourceRange()) +
                                      ", " + Rewrite.getRewrittenText(Cond->getSourceRange()) + ")";

                Rewrite.ReplaceText(Cond->getSourceRange(), newCond);
            }
        }
    }
}
  
void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = ifStmt(hasCondition(binaryOperator())).bind("ifStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}