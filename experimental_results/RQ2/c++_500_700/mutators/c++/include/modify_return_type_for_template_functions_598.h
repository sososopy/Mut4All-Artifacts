//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_return_type_for_template_functions_598
 */ 
class MutatorFrontendAction_598 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(598)

private:
    class MutatorASTConsumer_598 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_598(Rewriter &R) : TheRewriter(R) {}
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

