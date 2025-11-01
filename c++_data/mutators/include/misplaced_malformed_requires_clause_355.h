//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misplaced_Malformed_Requires_Clause_355
 */ 
class MutatorFrontendAction_355 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(355)

private:
    class MutatorASTConsumer_355 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_355(Rewriter &R) : TheRewriter(R) {}
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

