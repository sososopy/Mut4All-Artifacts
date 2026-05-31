//source file
#include "../include/modify_conditional_operator_with_throw_522.h"

// ========================================================================================================
#define MUT522_OUTPUT 1

void MutatorFrontendAction_522::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CO = Result.Nodes.getNodeAs<clang::ConditionalOperator>("CondOp")) {
        if (!CO || !Result.Context->getSourceManager().isWrittenInMainFile(
                CO->getBeginLoc()))
            return;

        auto expr1 = CO->getTrueExpr();
        auto expr2 = CO->getFalseExpr();

        std::string replacement;
        if (!llvm::isa<clang::CXXThrowExpr>(expr1)) {
            replacement = "throw 1";
            Rewrite.ReplaceText(expr1->getSourceRange(), replacement);
        } else if (!llvm::isa<clang::CXXThrowExpr>(expr2)) {
            replacement = "throw -1";
            Rewrite.ReplaceText(expr2->getSourceRange(), replacement);
        }
    }
}
  
void MutatorFrontendAction_522::MutatorASTConsumer_522::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conditionalOperator().bind("CondOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}