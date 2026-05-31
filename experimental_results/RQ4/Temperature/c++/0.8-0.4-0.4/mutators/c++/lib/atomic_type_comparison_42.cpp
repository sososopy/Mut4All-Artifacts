//source file
#include "../include/atomic_type_comparison_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("AtomicComparison")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
            return;

        auto LHS = BO->getLHS()->IgnoreImpCasts();
        auto RHS = BO->getRHS()->IgnoreImpCasts();

        if (auto *AtomicType = LHS->getType()->getAs<clang::AtomicType>()) {
            std::string replacement = (llvm::Twine(LHS->getStmtClassName()) + ".load() " + BO->getOpcodeStr().str() + " " + RHS->getStmtClassName()).str();
            Rewrite.ReplaceText(BO->getSourceRange(), replacement);
        } else if (auto *AtomicType = RHS->getType()->getAs<clang::AtomicType>()) {
            std::string replacement = (llvm::Twine(LHS->getStmtClassName()) + " " + BO->getOpcodeStr().str() + " " + RHS->getStmtClassName() + ".load()").str();
            Rewrite.ReplaceText(BO->getSourceRange(), replacement);
        }
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = binaryOperator(
        hasLHS(expr(hasType(atomicType()))),
        hasRHS(expr(unless(hasType(atomicType()))))
    ).bind("AtomicComparison");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}