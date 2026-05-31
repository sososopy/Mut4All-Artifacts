//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_embed_directive_553
 */ 
class MutatorFrontendAction_553 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(553)

private:
    class MutatorASTConsumer_553 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_553(Rewriter &R) : TheRewriter(R) {}
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

