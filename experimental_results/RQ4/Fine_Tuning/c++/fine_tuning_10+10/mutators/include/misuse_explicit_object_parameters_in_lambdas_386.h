//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Explicit_Object_Parameters_In_Lambdas_386
 */ 
class MutatorFrontendAction_386 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(386)

private:
    class MutatorASTConsumer_386 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_386(Rewriter &R) : TheRewriter(R) {}
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

