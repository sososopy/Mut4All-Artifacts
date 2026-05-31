//header file
#pragma once
#include "Mutator_base.h"

/**
 * Null_Base_Class_Dereference_486
 */ 
class MutatorFrontendAction_486 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(486)

private:
    class MutatorASTConsumer_486 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_486(Rewriter &R) : TheRewriter(R) {}
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

