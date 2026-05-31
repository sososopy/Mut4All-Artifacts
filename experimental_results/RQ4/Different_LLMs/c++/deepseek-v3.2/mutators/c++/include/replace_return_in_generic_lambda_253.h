//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Return_In_Generic_Lambda_253
 */ 
class MutatorFrontendAction_253 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(253)

private:
    class MutatorASTConsumer_253 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_253(Rewriter &R) : TheRewriter(R) {}
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

