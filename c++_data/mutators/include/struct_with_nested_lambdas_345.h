//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_With_Nested_Lambdas_345
 */ 
class MutatorFrontendAction_345 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(345)

private:
    class MutatorASTConsumer_345 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_345(Rewriter &R) : TheRewriter(R) {}
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

