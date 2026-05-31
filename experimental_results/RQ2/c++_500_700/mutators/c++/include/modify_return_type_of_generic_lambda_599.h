//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_return_type_of_generic_lambda_599
 */ 
class MutatorFrontendAction_599 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(599)

private:
    class MutatorASTConsumer_599 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_599(Rewriter &R) : TheRewriter(R) {}
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

