//header file
#pragma once
#include "Mutator_base.h"

/**
 * reorder_bitfield_initializers_678
 */ 
class MutatorFrontendAction_678 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(678)

private:
    class MutatorASTConsumer_678 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_678(Rewriter &R) : TheRewriter(R) {}
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

