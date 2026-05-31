//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Lambda_Parameters_426
 */ 
class MutatorFrontendAction_426 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(426)

private:
    class MutatorASTConsumer_426 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_426(Rewriter &R) : TheRewriter(R) {}
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

