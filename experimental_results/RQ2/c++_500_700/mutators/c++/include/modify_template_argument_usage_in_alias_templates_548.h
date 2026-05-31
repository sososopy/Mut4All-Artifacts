//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_argument_usage_in_alias_templates_548
 */ 
class MutatorFrontendAction_548 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(548)

private:
    class MutatorASTConsumer_548 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_548(Rewriter &R) : TheRewriter(R) {}
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

