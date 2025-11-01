//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_With_Decltype_381
 */ 
class MutatorFrontendAction_381 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(381)

private:
    class MutatorASTConsumer_381 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_381(Rewriter &R) : TheRewriter(R) {}
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

