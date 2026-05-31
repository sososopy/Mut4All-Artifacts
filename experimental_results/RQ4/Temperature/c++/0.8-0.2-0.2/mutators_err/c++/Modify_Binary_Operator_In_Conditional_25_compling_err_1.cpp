//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_binary_operator_in_conditional_25
 */ 
class MutatorFrontendAction_25 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(25)

private:
    class MutatorASTConsumer_25 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_25(Rewriter &R) : TheRewriter(R) {}
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
            std::string newExpr = "(" + lhs->getStmtClassName() + " < reinterpret_cast<int>(" + rhs->getStmtClassName() + "))";
            Rewrite.ReplaceText(BO->getSourceRange(), newExpr);
        } else if (!lhsType->isIntegerType() && rhsType->isIntegerType()) {
            std::string newExpr = "(reinterpret_cast<int>(" + lhs->getStmtClassName() + ") < " + rhs->getStmtClassName() + ")";
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