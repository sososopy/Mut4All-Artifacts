//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Function_Call_Argument_292
 */ 
class MutatorFrontendAction_292 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(292)

private:
    class MutatorASTConsumer_292 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_292(Rewriter &R) : TheRewriter(R) {}
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

