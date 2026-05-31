//header file
#pragma once
#include "Mutator_base.h"

/**
 * invalid_template_deduction_guide_514
 */ 
class MutatorFrontendAction_514 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(514)

private:
    class MutatorASTConsumer_514 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_514(Rewriter &R) : TheRewriter(R) {}
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

