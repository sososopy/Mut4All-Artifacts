//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_in_immediate_function_call_618
 */ 
class MutatorFrontendAction_618 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(618)

private:
    class MutatorASTConsumer_618 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_618(Rewriter &R) : TheRewriter(R) {}
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

