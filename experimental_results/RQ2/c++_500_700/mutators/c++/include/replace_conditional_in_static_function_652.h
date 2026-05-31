//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_conditional_in_static_function_652
 */ 
class MutatorFrontendAction_652 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(652)

private:
    class MutatorASTConsumer_652 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_652(Rewriter &R) : TheRewriter(R) {}
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

