//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Body_With_Empty_281
 */ 
class MutatorFrontendAction_281 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(281)

private:
    class MutatorASTConsumer_281 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_281(Rewriter &R) : TheRewriter(R) {}
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

