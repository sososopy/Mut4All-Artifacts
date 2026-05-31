//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_parameter_list_511
 */ 
class MutatorFrontendAction_511 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(511)

private:
    class MutatorASTConsumer_511 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_511(Rewriter &R) : TheRewriter(R) {}
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

