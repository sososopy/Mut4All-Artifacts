//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Parentheses_In_Macro_317
 */ 
class MutatorFrontendAction_317 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(317)

private:
    class MutatorASTConsumer_317 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_317(Rewriter &R) : TheRewriter(R) {}
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

