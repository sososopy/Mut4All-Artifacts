//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Nested_Forwarding_Helper_397
 */ 
class MutatorFrontendAction_397 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(397)

private:
    class MutatorASTConsumer_397 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_397(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        bool helperInserted = false;
    };
};

