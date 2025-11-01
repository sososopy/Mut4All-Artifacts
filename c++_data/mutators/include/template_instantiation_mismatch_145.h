//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Instantiation_Mismatch_145
 */ 
class MutatorFrontendAction_145 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(145)

private:
    class MutatorASTConsumer_145 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_145(Rewriter &R) : TheRewriter(R) {}
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

