//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Concept_Requirements_339
 */ 
class MutatorFrontendAction_339 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(339)

private:
    class MutatorASTConsumer_339 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_339(Rewriter &R) : TheRewriter(R) {}
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

