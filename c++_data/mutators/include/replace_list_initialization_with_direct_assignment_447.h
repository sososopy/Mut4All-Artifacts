//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_List_Initialization_With_Direct_Assignment_447
 */ 
class MutatorFrontendAction_447 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(447)

private:
    class MutatorASTConsumer_447 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_447(Rewriter &R) : TheRewriter(R) {}
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

