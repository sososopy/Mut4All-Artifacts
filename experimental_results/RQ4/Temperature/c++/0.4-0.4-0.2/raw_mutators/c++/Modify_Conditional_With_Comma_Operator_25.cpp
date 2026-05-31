//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_conditional_with_comma_operator_25
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
                std::string newCond = "(" + TheRewriter.getRewrittenText(LHS->getSourceRange()) +
                                      " = " + TheRewriter.getRewrittenText(RHS->getSourceRange()) +
                                      ", " + TheRewriter.getRewrittenText(Cond->getSourceRange()) + ")";

                TheRewriter.ReplaceText(Cond->getSourceRange(), newCond);
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