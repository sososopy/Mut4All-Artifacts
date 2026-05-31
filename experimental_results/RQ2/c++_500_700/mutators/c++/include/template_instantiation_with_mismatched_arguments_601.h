//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_instantiation_with_mismatched_arguments_601
 */ 
class MutatorFrontendAction_601 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(601)

private:
    class MutatorASTConsumer_601 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_601(Rewriter &R) : TheRewriter(R) {}
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

