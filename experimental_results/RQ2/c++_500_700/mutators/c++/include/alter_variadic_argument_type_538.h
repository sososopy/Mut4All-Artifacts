//header file
#pragma once
#include "Mutator_base.h"

/**
 * alter_variadic_argument_type_538
 */ 
class MutatorFrontendAction_538 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(538)

private:
    class MutatorASTConsumer_538 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_538(Rewriter &R) : TheRewriter(R) {}
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

