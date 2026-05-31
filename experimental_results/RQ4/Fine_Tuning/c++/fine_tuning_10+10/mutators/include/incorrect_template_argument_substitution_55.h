//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incorrect_Template_Argument_Substitution_55
 */ 
class MutatorFrontendAction_55 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(55)

private:
    class MutatorASTConsumer_55 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_55(Rewriter &R) : TheRewriter(R) {}
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

