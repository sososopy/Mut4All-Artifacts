//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Concept_Constraint_And_Partial_Specialization_135
 */ 
class MutatorFrontendAction_135 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(135)

private:
    class MutatorASTConsumer_135 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_135(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

