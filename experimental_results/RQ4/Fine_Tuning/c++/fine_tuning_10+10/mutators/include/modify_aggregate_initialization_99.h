//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Aggregate_Initialization_99
 */ 
class MutatorFrontendAction_99 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(99)

private:
    class MutatorASTConsumer_99 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_99(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

