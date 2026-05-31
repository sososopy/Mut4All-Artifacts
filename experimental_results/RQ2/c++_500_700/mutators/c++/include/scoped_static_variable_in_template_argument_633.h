//header file
#pragma once
#include "Mutator_base.h"

/**
 * scoped_static_variable_in_template_argument_633
 */ 
class MutatorFrontendAction_633 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(633)

private:
    class MutatorASTConsumer_633 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_633(Rewriter &R) : TheRewriter(R) {}
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

