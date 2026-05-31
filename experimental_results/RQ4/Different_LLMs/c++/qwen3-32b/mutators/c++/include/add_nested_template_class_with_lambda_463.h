//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Nested_Template_Class_With_Lambda_463
 */ 
class MutatorFrontendAction_463 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(463)

private:
    class MutatorASTConsumer_463 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_463(Rewriter &R) : TheRewriter(R) {}
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

