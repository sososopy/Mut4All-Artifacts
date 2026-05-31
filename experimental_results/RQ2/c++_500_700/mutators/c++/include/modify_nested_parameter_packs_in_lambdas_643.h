//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_nested_parameter_packs_in_lambdas_643
 */ 
class MutatorFrontendAction_643 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(643)

private:
    class MutatorASTConsumer_643 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_643(Rewriter &R) : TheRewriter(R) {}
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

