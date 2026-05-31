//header file
#pragma once
#include "Mutator_base.h"

/**
 * invalid_conditional_operator_in_range_loop_669
 */ 
class MutatorFrontendAction_669 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(669)

private:
    class MutatorASTConsumer_669 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_669(Rewriter &R) : TheRewriter(R) {}
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

