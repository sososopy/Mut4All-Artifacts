//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Unreachable_Code_Block_459
 */ 
class MutatorFrontendAction_459 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(459)

private:
    class MutatorASTConsumer_459 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_459(Rewriter &R) : TheRewriter(R) {}
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

