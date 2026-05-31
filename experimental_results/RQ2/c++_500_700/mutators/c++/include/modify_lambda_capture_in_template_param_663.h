//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_capture_in_template_param_663
 */ 
class MutatorFrontendAction_663 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(663)

private:
    class MutatorASTConsumer_663 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_663(Rewriter &R) : TheRewriter(R) {}
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

