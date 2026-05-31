//header file
#pragma once
#include "Mutator_base.h"

/**
 * incorrect_template_deduction_guide_586
 */ 
class MutatorFrontendAction_586 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(586)

private:
    class MutatorASTConsumer_586 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_586(Rewriter &R) : TheRewriter(R) {}
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

