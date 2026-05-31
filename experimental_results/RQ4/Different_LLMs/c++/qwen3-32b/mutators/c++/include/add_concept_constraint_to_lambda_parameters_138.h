//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Concept_Constraint_To_Lambda_Parameters_138
 */ 
class MutatorFrontendAction_138 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(138)

private:
    class MutatorASTConsumer_138 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_138(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

