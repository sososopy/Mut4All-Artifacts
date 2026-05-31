//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_function_call_arguments_596
 */ 
class MutatorFrontendAction_596 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(596)

private:
    class MutatorASTConsumer_596 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_596(Rewriter &R) : TheRewriter(R) {}
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

