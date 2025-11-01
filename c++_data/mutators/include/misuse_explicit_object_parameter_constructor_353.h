//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Explicit_Object_Parameter_Constructor_353
 */ 
class MutatorFrontendAction_353 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(353)

private:
    class MutatorASTConsumer_353 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_353(Rewriter &R) : TheRewriter(R) {}
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

