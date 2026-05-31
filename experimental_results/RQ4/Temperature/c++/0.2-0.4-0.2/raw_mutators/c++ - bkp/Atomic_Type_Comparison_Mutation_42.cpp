//header file
#pragma once
#include "Mutator_base.h"

/**
 * atomic_type_comparison_mutation_42
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
#include "../include/atomic_type_comparison_mutation_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("atomicComparison")) {
        //Filter nodes in header files
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
            return;

        auto lhs = BO->getLHS()->IgnoreParenCasts();
        auto rhs = BO->getRHS()->IgnoreParenCasts();

        const AtomicType *atomicType = nullptr;
        const Expr *nonAtomicExpr = nullptr;

        if (lhs->getType()->isAtomicType()) {
            atomicType = lhs->getType()->getAs<AtomicType>();
            nonAtomicExpr = rhs;
        } else if (rhs->getType()->isAtomicType()) {
            atomicType = rhs->getType()->getAs<AtomicType>();
            nonAtomicExpr = lhs;
        }

        if (atomicType && nonAtomicExpr) {
            //Get the source code text of target node
            auto atomicExprStr = stringutils::rangetoStr(*(Result.SourceManager), lhs->getSourceRange());
            auto nonAtomicExprStr = stringutils::rangetoStr(*(Result.SourceManager), nonAtomicExpr->getSourceRange());

            //Perform mutation on the source code text by applying string replacement
            std::string castedExpr;
            if (lhs->getType()->isAtomicType()) {
                castedExpr = "static_cast<" + atomicType->getValueType().getAsString() + ">(" + atomicExprStr + ")";
            } else {
                castedExpr = "static_cast<" + nonAtomicExpr->getType().getAsString() + ">(" + atomicExprStr + ")";
            }

            std::string mutatedComparison = castedExpr + " == " + nonAtomicExprStr;

            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(BO->getSourceRange(), mutatedComparison);
        }
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(hasOperatorName("=="),
                                              hasLHS(expr(anyOf(hasType(isAtomicType()), unless(hasType(isAtomicType()))))),
                                              hasRHS(expr(anyOf(hasType(isAtomicType()), unless(hasType(isAtomicType())))))
                                             ).bind("atomicComparison");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}