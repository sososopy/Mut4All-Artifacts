//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_specialization_incomplete_type_690
 */ 
class MutatorFrontendAction_690 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(690)

private:
    class MutatorASTConsumer_690 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_690(Rewriter &R) : TheRewriter(R) {}
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

