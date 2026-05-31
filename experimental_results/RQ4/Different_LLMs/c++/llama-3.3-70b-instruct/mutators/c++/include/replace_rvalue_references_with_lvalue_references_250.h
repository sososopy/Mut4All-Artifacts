//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Rvalue_References_With_Lvalue_References_250
 */ 
class MutatorFrontendAction_250 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(250)

private:
    class MutatorASTConsumer_250 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_250(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

