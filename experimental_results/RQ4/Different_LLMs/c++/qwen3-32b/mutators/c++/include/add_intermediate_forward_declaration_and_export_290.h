//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Intermediate_Forward_Declaration_And_Export_290
 */ 
class MutatorFrontendAction_290 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(290)

private:
    class MutatorASTConsumer_290 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_290(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

