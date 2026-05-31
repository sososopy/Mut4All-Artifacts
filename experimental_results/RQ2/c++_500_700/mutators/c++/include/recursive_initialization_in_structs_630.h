//header file
#pragma once
#include "Mutator_base.h"

/**
 * recursive_initialization_in_structs_630
 */ 
class MutatorFrontendAction_630 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(630)

private:
    class MutatorASTConsumer_630 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_630(Rewriter &R) : TheRewriter(R) {}
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

