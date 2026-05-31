//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_capture_in_nested_contexts_534
 */ 
class MutatorFrontendAction_534 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(534)

private:
    class MutatorASTConsumer_534 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_534(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> outerVariables;
    };
};

