//header file
#pragma once
#include "Mutator_base.h"

/**
 * lambda_with_nested_requires_687
 */ 
class MutatorFrontendAction_687 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(687)

private:
    class MutatorASTConsumer_687 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_687(Rewriter &R) : TheRewriter(R) {}
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

