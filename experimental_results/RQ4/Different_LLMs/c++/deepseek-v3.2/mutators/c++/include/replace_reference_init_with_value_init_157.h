//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Reference_Init_With_Value_Init_157
 */ 
class MutatorFrontendAction_157 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(157)

private:
    class MutatorASTConsumer_157 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_157(Rewriter &R) : TheRewriter(R) {}
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

