//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Conditional_Operators_139
 */ 
class MutatorFrontendAction_139 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(139)

private:
    class MutatorASTConsumer_139 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_139(Rewriter &R) : TheRewriter(R) {}
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

