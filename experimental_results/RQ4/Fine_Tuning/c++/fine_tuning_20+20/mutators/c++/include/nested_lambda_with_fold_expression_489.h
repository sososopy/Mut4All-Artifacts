//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Lambda_with_Fold_Expression_489
 */ 
class MutatorFrontendAction_489 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(489)

private:
    class MutatorASTConsumer_489 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_489(Rewriter &R) : TheRewriter(R) {}
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

