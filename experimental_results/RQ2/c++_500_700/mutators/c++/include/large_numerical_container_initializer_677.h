//header file
#pragma once
#include "Mutator_base.h"

/**
 * large_numerical_container_initializer_677
 */ 
class MutatorFrontendAction_677 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(677)

private:
    class MutatorASTConsumer_677 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_677(Rewriter &R) : TheRewriter(R) {}
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

