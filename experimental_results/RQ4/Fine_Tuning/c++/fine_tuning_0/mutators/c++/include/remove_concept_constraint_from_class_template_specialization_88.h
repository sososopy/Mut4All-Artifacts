//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Concept_Constraint_From_Class_Template_Specialization_88
 */ 
class MutatorFrontendAction_88 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(88)

private:
    class MutatorASTConsumer_88 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_88(Rewriter &R) : TheRewriter(R) {}
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

