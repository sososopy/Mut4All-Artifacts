//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_305
 */ 
class MutatorFrontendAction_305 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(305)

private:
    class MutatorASTConsumer_305 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_305(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

