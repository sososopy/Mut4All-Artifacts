//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Concept_Parameter_Require_Clause_68
 */ 
class MutatorFrontendAction_68 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(68)

private:
    class MutatorASTConsumer_68 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_68(Rewriter &R) : TheRewriter(R) {}
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

