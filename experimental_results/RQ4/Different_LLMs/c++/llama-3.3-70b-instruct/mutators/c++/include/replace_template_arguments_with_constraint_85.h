//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Arguments_With_Constraint_85
 */ 
class MutatorFrontendAction_85 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(85)

private:
    class MutatorASTConsumer_85 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_85(Rewriter &R) : TheRewriter(R) {}
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

