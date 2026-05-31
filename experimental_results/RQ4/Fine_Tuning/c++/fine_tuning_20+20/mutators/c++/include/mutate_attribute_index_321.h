//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Attribute_Index_321
 */ 
class MutatorFrontendAction_321 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(321)

private:
    class MutatorASTConsumer_321 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_321(Rewriter &R) : TheRewriter(R) {}
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

