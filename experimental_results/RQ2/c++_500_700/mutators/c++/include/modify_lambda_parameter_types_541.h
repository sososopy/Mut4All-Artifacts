//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_parameter_types_541
 */ 
class MutatorFrontendAction_541 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(541)

private:
    class MutatorASTConsumer_541 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_541(Rewriter &R) : TheRewriter(R) {}
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

