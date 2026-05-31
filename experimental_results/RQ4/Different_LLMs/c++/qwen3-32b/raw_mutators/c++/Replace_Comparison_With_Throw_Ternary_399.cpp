//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Comparison_With_Throw_Ternary_399
 */ 
class MutatorFrontendAction_399 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(399)

private:
    class MutatorASTConsumer_399 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_399(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Comparison_With_Throw_Ternary_399.h"

// ========================================================================================================
#define MUT399_OUTPUT 1

void MutatorFrontendAction_399::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("compOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(BO->getLocation()))
            return;

        // Randomly select left or right operand
        bool selectLeft = getrandom::getRandomIndex(2) == 0;
        const Expr *operand = selectLeft ? BO->getLHS() : BO->getRHS();
        const Expr *otherOperand = selectLeft ? BO->getRHS() : BO->getLHS();

        // Get operand type and generate throw expression
        QualType operandType = operand->getType();
        std::string throwExpr;
        if (operandType->isIntegerType()) {
            throwExpr = "0";
        } else if (operandType->isFloatingType()) {
            throwExpr = "3.14";
        } else if (operandType->isBooleanType()) {
            throwExpr = "false";
        } else {
            throwExpr = "0";
        }

        // Get original operand text
        std::string originalOperand = stringutils::rangetoStr(*Result.SourceManager, operand->getSourceRange());
        std::string condition = "true";
        std::string ternaryExpr = "(" + condition + " ? throw " + throwExpr + " : " + originalOperand + ")";

        // Replace selected operand
        Rewrite.ReplaceText(operand->getSourceRange(), ternaryExpr);
    }
}
  
void MutatorFrontendAction_399::MutatorASTConsumer_399::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = binaryOperator().bind("compOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}