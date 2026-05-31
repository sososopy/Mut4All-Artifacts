//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Statement_365
 */ 
class MutatorFrontendAction_365 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(365)

private:
    class MutatorASTConsumer_365 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_365(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

