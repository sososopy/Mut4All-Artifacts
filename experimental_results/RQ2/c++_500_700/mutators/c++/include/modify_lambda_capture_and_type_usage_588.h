//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_capture_and_type_usage_588
 */ 
class MutatorFrontendAction_588 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(588)

private:
    class MutatorASTConsumer_588 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_588(Rewriter &R) : TheRewriter(R) {}
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

