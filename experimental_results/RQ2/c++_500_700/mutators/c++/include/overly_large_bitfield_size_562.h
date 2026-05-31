//header file
#pragma once
#include "Mutator_base.h"

/**
 * overly_large_bitfield_size_562
 */ 
class MutatorFrontendAction_562 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(562)

private:
    class MutatorASTConsumer_562 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_562(Rewriter &R) : TheRewriter(R) {}
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

