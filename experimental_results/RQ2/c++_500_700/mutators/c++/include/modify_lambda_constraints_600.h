//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_constraints_600
 */ 
class MutatorFrontendAction_600 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(600)

private:
    class MutatorASTConsumer_600 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_600(Rewriter &R) : TheRewriter(R) {}
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

