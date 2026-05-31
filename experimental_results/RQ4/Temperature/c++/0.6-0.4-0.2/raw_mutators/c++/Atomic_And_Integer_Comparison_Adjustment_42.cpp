//header file
#pragma once
#include "Mutator_base.h"

/**
 * atomic_and_integer_comparison_adjustment_42
 */ 
class MutatorFrontendAction_42 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(42)

private:
    class MutatorASTConsumer_42 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_42(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/atomic_and_integer_comparison_adjustment_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("comparisonOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
            return;

        auto LHS = BO->getLHS();
        auto RHS = BO->getRHS();
        const Expr *atomicExpr = nullptr;
        const Expr *nonAtomicExpr = nullptr;

        if (LHS->getType()->isAtomicType() && !RHS->getType()->isAtomicType()) {
            atomicExpr = LHS;
            nonAtomicExpr = RHS;
        } else if (RHS->getType()->isAtomicType() && !LHS->getType()->isAtomicType()) {
            atomicExpr = RHS;
            nonAtomicExpr = LHS;
        }

        if (atomicExpr && nonAtomicExpr) {
            std::string atomicText = Lexer::getSourceText(
                CharSourceRange::getTokenRange(atomicExpr->getSourceRange()),
                *Result.SourceManager, Result.Context->getLangOpts()).str();

            std::string replacementText = atomicText + ".load()";
            Rewrite.ReplaceText(atomicExpr->getSourceRange(), replacementText);
        }
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = binaryOperator(
        hasOperatorName(">"),
        hasLHS(expr().bind("lhs")),
        hasRHS(expr().bind("rhs")),
        anyOf(
            hasLHS(hasType(atomicType())),
            hasRHS(hasType(atomicType()))
        )
    ).bind("comparisonOp");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}