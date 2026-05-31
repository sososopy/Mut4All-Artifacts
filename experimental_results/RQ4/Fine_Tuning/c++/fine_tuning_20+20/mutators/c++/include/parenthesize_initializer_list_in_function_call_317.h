//header file
#pragma once
#include "Mutator_base.h"

/**
 * Parenthesize_Initializer_List_In_Function_Call_317
 */ 
class MutatorFrontendAction_317 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(317)

private:
    class MutatorASTConsumer_317 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_317(Rewriter &R) : TheRewriter(R) {}
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

