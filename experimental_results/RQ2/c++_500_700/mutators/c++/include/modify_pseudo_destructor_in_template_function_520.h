//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_pseudo_destructor_in_template_function_520
 */ 
class MutatorFrontendAction_520 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(520)

private:
    class MutatorASTConsumer_520 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_520(Rewriter &R) : TheRewriter(R) {}
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

