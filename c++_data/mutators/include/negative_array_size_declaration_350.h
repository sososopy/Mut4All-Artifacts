//header file
#pragma once
#include "Mutator_base.h"

/**
 * Negative_Array_Size_Declaration_350
 */ 
class MutatorFrontendAction_350 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(350)

private:
    class MutatorASTConsumer_350 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_350(Rewriter &R) : TheRewriter(R) {}
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

