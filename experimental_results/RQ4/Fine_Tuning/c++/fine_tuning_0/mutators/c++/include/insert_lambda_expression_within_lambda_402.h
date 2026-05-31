//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_Expression_Within_Lambda_402
 */ 
class MutatorFrontendAction_402 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(402)

private:
    class MutatorASTConsumer_402 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_402(Rewriter &R) : TheRewriter(R) {}
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

