//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Template_Function_Declaration_379
 */ 
class MutatorFrontendAction_379 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(379)

private:
    class MutatorASTConsumer_379 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_379(Rewriter &R) : TheRewriter(R) {}
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

