//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_460
 */ 
class MutatorFrontendAction_460 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(460)

private:
    class MutatorASTConsumer_460 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_460(Rewriter &R) : TheRewriter(R) {}
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

