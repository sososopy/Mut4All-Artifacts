//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Operator_With_Function_Call_395
 */ 

class MutatorFrontendAction_395 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(395)
private:
    class MutatorASTConsumer_395 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_395(Rewriter &R) : TheRewriter(R) {}
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
    };
};

