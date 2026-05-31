//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Virtual_Function_With_Pure_Virtual_171
 */ 
class MutatorFrontendAction_171 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(171)

private:
    class MutatorASTConsumer_171 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_171(Rewriter &R) : TheRewriter(R) {}
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

