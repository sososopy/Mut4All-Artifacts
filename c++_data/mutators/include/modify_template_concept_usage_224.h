//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Concept_Usage_224
 */ 
class MutatorFrontendAction_224 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(224)

private:
    class MutatorASTConsumer_224 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_224(Rewriter &R) : TheRewriter(R) {}
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

