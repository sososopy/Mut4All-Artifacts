//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Function_Call_387
 */ 
class MutatorFrontendAction_387 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(387)

private:
    class MutatorASTConsumer_387 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_387(Rewriter &R) : TheRewriter(R) {}
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

