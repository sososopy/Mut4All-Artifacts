//header file
#pragma once
#include "Mutator_base.h"

/**
 * UCN_To_Raw_Character_457
 */ 
class MutatorFrontendAction_457 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(457)

private:
    class MutatorASTConsumer_457 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_457(Rewriter &R) : TheRewriter(R) {}
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

