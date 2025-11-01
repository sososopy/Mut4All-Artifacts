//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inheritance_With_Multiple_Generic_Lambdas_22
 */ 
class MutatorFrontendAction_22 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(22)

private:
    class MutatorASTConsumer_22 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_22(Rewriter &R) : TheRewriter(R) {}
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

