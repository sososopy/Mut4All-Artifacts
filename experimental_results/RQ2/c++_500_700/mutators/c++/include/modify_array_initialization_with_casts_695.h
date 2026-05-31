//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_array_initialization_with_casts_695
 */ 
class MutatorFrontendAction_695 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(695)

private:
    class MutatorASTConsumer_695 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_695(Rewriter &R) : TheRewriter(R) {}
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

