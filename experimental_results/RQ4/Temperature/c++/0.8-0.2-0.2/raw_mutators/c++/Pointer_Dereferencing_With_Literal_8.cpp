//header file
#pragma once
#include "Mutator_base.h"

/**
 * pointer_dereferencing_with_literal_8
 */ 
class MutatorFrontendAction_8 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(8)

private:
    class MutatorASTConsumer_8 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_8(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/pointer_dereferencing_with_literal_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *DerefExpr = Result.Nodes.getNodeAs<clang::UnaryOperator>("DerefExpr")) {
        // Filter nodes in header files
        if (!DerefExpr || !Result.Context->getSourceManager().isWrittenInMainFile(DerefExpr->getBeginLoc()))
            return;
        
        // Ensure it's a dereference operation
        if (DerefExpr->getOpcode() == clang::UO_Deref) {
            // Perform mutation by replacing the pointer operand with a literal
            auto DerefSourceRange = DerefExpr->getSourceRange();
            Rewrite.ReplaceText(DerefSourceRange, "*0");
        }
    }
}

void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = unaryOperator(hasOperatorName("*"), hasUnaryOperand(ignoringParenImpCasts(declRefExpr(to(varDecl(hasType(pointerType()))))))).bind("DerefExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}