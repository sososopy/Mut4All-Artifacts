//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Function_Overloading_326
 */ 
class MutatorFrontendAction_326 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(326)

private:
    class MutatorASTConsumer_326 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_326(Rewriter &R) : TheRewriter(R) {}
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

