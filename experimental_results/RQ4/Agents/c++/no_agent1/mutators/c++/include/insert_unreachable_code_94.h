//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_94
 */ 
class MutatorFrontendAction_94 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(94)

private:
    class MutatorASTConsumer_94 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_94(Rewriter &R) : TheRewriter(R) {}
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

