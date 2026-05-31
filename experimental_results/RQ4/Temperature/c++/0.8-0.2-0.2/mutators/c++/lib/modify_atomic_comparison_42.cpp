//source file
#include "../include/modify_atomic_comparison_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("atomicComparison")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(BO->getExprLoc()))
            return;

        const Expr *lhs = BO->getLHS()->IgnoreParenImpCasts();
        const Expr *rhs = BO->getRHS()->IgnoreParenImpCasts();

        const AtomicType *atomicType = nullptr;
        const Expr *nonAtomicExpr = nullptr;

        if (lhs->getType()->isAtomicType() && !rhs->getType()->isAtomicType()) {
            atomicType = lhs->getType()->getAs<AtomicType>();
            nonAtomicExpr = rhs;
        } else if (rhs->getType()->isAtomicType() && !lhs->getType()->isAtomicType()) {
            atomicType = rhs->getType()->getAs<AtomicType>();
            nonAtomicExpr = lhs;
        }

        if (!atomicType || !nonAtomicExpr)
            return;

        std::string atomicTypeName = atomicType->getValueType()->getCanonicalTypeInternal().getAsString();
        std::string nonAtomicExprText = Lexer::getSourceText(CharSourceRange::getTokenRange(nonAtomicExpr->getSourceRange()), 
                                                             *Result.SourceManager, 
                                                             Result.Context->getLangOpts()).str();

        std::string replacementText = "std::atomic<" + atomicTypeName + ">(" + nonAtomicExprText + ")";
        Rewrite.ReplaceText(nonAtomicExpr->getSourceRange(), replacementText);
    }
}

void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto atomicComparisonMatcher = binaryOperator(
        anyOf(hasLHS(expr(hasType(atomicType()))), hasRHS(expr(hasType(atomicType())))),
        anyOf(hasLHS(expr(unless(hasType(atomicType())))), hasRHS(expr(unless(hasType(atomicType())))))
    ).bind("atomicComparison");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(atomicComparisonMatcher, &callback);
    matchFinder.matchAST(Context);
}