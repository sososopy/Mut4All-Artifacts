//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_With_Invalid_ASM_Constraint_273
 */ 
class MutatorFrontendAction_273 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(273)

private:
    class MutatorASTConsumer_273 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_273(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_With_Invalid_ASM_Constraint_273.h"

// ========================================================================================================
#define MUT273_OUTPUT 1

void MutatorFrontendAction_273::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("expr")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(BO->getBeginLoc()))
            return;

        if (BO->getOpcode() != clang::BinaryOperator::BO_Assign)
            return;

        auto lhs = BO->getLHS();
        auto rhs = BO->getRHS();

        if (auto *lhsDRE = dyn_cast<clang::DeclRefExpr>(lhs)) {
            QualType lhsType = lhsDRE->getType();
            if (!lhsType->isIntegerType()) {
                if (auto *rhsBO = dyn_cast<clang::BinaryOperator>(rhs)) {
                    if (rhsBO->getOpcode() == clang::BinaryOperator::BO_Add) {
                        auto rhsLHS = rhsBO->getLHS();
                        auto rhsRHS = rhsBO->getRHS();

                        if (auto *rhsLHSDRE = dyn_cast<clang::DeclRefExpr>(rhsLHS) &&
                            auto *rhsRHSDRE = dyn_cast<clang::DeclRefExpr>(rhsRHS)) {
                            std::string lhsVar = lhsDRE->getName().str();
                            std::string y = rhsLHSDRE->getName().str();
                            std::string z = rhsRHSDRE->getName().str();

                            std::string asmStr = "asm (\"\" : \"=q\"(" + lhsVar + ") : \"r\"(" + y + "), \"r\"(" + z + "));";
                            Rewrite.ReplaceText(BO->getSourceRange(), asmStr);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_273::MutatorASTConsumer_273::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = binaryOperator(hasOperatorName("=")).bind("expr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}