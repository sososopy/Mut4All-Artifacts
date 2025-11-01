//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Declval_In_Lambda_36
 */ 
class MutatorFrontendAction_36 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(36)

private:
    class MutatorASTConsumer_36 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_36(Rewriter &R) : TheRewriter(R) {}
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

