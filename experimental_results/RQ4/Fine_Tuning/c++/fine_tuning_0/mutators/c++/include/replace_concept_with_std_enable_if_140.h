//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_With_Std_Enable_If_140
 */ 
class MutatorFrontendAction_140 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(140)

private:
    class MutatorASTConsumer_140 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_140(Rewriter &R) : TheRewriter(R) {}
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

