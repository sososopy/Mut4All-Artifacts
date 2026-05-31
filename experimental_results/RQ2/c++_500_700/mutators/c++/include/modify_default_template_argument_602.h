//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_default_template_argument_602
 */ 
class MutatorFrontendAction_602 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(602)

private:
    class MutatorASTConsumer_602 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_602(Rewriter &R) : TheRewriter(R) {}
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

