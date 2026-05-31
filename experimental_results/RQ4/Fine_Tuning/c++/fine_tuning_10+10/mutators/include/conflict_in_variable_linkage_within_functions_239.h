//header file
#pragma once
#include "Mutator_base.h"

/**
 * Conflict_In_Variable_Linkage_Within_Functions_239
 */ 
class MutatorFrontendAction_239 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(239)

private:
    class MutatorASTConsumer_239 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_239(Rewriter &R) : TheRewriter(R) {}
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

