//header file
#pragma once
#include "Mutator_base.h"

/**
 * lambda_auto_initializer_674
 */ 
class MutatorFrontendAction_674 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(674)

private:
    class MutatorASTConsumer_674 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_674(Rewriter &R) : TheRewriter(R) {}
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

