//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Union_Initialization_322
 */ 
class MutatorFrontendAction_322 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(322)

private:
    class MutatorASTConsumer_322 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_322(Rewriter &R) : TheRewriter(R) {}
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

