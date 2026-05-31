//header file
#pragma once
#include "Mutator_base.h"

/**
 * simplify_array_size_expression_606
 */ 
class MutatorFrontendAction_606 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(606)

private:
    class MutatorASTConsumer_606 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_606(Rewriter &R) : TheRewriter(R) {}
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

