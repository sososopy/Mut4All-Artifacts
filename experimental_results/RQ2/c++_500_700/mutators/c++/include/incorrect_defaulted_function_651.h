//header file
#pragma once
#include "Mutator_base.h"

/**
 * incorrect_defaulted_function_651
 */ 
class MutatorFrontendAction_651 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(651)

private:
    class MutatorASTConsumer_651 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_651(Rewriter &R) : TheRewriter(R) {}
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

