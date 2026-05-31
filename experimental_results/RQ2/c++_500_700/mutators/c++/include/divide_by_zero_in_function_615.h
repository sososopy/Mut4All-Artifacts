//header file
#pragma once
#include "Mutator_base.h"

/**
 * divide_by_zero_in_function_615
 */ 
class MutatorFrontendAction_615 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(615)

private:
    class MutatorASTConsumer_615 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_615(Rewriter &R) : TheRewriter(R) {}
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

