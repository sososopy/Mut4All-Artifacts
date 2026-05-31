//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Base_Class_Function_Call_With_Static_Cast_274
 */ 
class MutatorFrontendAction_274 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(274)

private:
    class MutatorASTConsumer_274 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_274(Rewriter &R) : TheRewriter(R) {}
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

