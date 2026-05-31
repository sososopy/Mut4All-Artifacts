//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Static_Assertion_208
 */ 
class MutatorFrontendAction_208 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(208)

private:
    class MutatorASTConsumer_208 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_208(Rewriter &R) : TheRewriter(R) {}
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

