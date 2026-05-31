//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_instantiation_with_incomplete_types_573
 */ 
class MutatorFrontendAction_573 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(573)

private:
    class MutatorASTConsumer_573 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_573(Rewriter &R) : TheRewriter(R) {}
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

