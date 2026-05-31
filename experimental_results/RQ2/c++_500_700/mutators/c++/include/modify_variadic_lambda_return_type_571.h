//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_variadic_lambda_return_type_571
 */ 
class MutatorFrontendAction_571 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(571)

private:
    class MutatorASTConsumer_571 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_571(Rewriter &R) : TheRewriter(R) {}
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

