//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Method_Call_With_Null_Assignment_190
 */ 
class MutatorFrontendAction_190 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(190)

private:
    class MutatorASTConsumer_190 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_190(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Method_Call_With_Null_Assignment_190.h"

// ========================================================================================================
#define MUT190_OUTPUT 1

void MutatorFrontendAction_190::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *assignExpr = Result.Nodes.getNodeAs<clang::AssignExpr>("assignExpr")) {
        if (!assignExpr || !Result.Context->getSourceManager().isWrittenInMainFile(assignExpr->getBeginLoc()))
            return;
        
        clang::Expr *rhs = assignExpr->getRHS();
        if (!rhs)
            return;
        
        Rewrite.ReplaceText(rhs->getSourceRange(), "NULL");
    }
}

void MutatorFrontendAction_190::MutatorASTConsumer_190::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = assignmentExpr(
        hasLHS(declRefExpr()),
        hasRHS(memberExpr())
    ).bind("assignExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}