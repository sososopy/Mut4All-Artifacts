//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Misuse_270
 */ 
class MutatorFrontendAction_270 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(270)

private:
    class MutatorASTConsumer_270 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_270(Rewriter &R) : TheRewriter(R) {}
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

