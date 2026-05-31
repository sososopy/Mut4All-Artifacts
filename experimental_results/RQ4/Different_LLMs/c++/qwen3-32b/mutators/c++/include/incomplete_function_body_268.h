//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Function_Body_268
 */ 
class MutatorFrontendAction_268 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(268)

private:
    class MutatorASTConsumer_268 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_268(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

