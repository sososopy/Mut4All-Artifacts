//header file
#pragma once
#include "Mutator_base.h"

/**
 * invalid_builtin_function_arguments_519
 */ 
class MutatorFrontendAction_519 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(519)

private:
    class MutatorASTConsumer_519 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_519(Rewriter &R) : TheRewriter(R) {}
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

