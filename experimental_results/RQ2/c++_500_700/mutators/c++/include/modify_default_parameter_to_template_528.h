//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_default_parameter_to_template_528
 */ 
class MutatorFrontendAction_528 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(528)

private:
    class MutatorASTConsumer_528 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_528(Rewriter &R) : TheRewriter(R) {}
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

