//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Requires_Clause_400
 */ 
class MutatorFrontendAction_400 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(400)

private:
    class MutatorASTConsumer_400 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_400(Rewriter &R) : TheRewriter(R) {}
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

