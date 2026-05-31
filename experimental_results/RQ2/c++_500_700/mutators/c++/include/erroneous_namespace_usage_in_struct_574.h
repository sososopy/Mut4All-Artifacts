//header file
#pragma once
#include "Mutator_base.h"

/**
 * erroneous_namespace_usage_in_struct_574
 */ 
class MutatorFrontendAction_574 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(574)

private:
    class MutatorASTConsumer_574 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_574(Rewriter &R) : TheRewriter(R) {}
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

