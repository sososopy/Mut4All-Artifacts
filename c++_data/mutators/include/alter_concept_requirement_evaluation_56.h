//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Concept_Requirement_Evaluation_56
 */ 
class MutatorFrontendAction_56 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(56)

private:
    class MutatorASTConsumer_56 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_56(Rewriter &R) : TheRewriter(R) {}
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

