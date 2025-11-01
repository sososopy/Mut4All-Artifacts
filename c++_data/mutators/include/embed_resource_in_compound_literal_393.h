//header file
#pragma once
#include "Mutator_base.h"

/**
 * Embed_Resource_In_Compound_Literal_393
 */ 
class MutatorFrontendAction_393 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(393)

private:
    class MutatorASTConsumer_393 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_393(Rewriter &R) : TheRewriter(R) {}
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

