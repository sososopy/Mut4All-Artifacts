//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Constructor_Parameter_187
 */ 
class MutatorFrontendAction_187 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(187)

private:
    class MutatorASTConsumer_187 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_187(Rewriter &R) : TheRewriter(R) {}
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

