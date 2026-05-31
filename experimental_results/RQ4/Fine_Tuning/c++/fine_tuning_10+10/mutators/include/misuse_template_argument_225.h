//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Template_Argument_225
 */ 
class MutatorFrontendAction_225 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(225)

private:
    class MutatorASTConsumer_225 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_225(Rewriter &R) : TheRewriter(R) {}
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

