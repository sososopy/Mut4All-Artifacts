//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_union_initialization_with_multiple_designators_531
 */ 
class MutatorFrontendAction_531 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(531)

private:
    class MutatorASTConsumer_531 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_531(Rewriter &R) : TheRewriter(R) {}
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

