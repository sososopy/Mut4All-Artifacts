//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_requires_clause_with_ambiguous_expression_700
 */ 
class MutatorFrontendAction_700 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(700)

private:
    class MutatorASTConsumer_700 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_700(Rewriter &R) : TheRewriter(R) {}
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

