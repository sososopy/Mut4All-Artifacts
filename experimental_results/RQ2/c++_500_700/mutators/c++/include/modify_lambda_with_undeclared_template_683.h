//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_with_undeclared_template_683
 */ 
class MutatorFrontendAction_683 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(683)

private:
    class MutatorASTConsumer_683 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_683(Rewriter &R) : TheRewriter(R) {}
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

