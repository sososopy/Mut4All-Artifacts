//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_parameter_packs_and_concepts_631
 */ 
class MutatorFrontendAction_631 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(631)

private:
    class MutatorASTConsumer_631 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_631(Rewriter &R) : TheRewriter(R) {}
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

