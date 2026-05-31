//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_constexpr_with_non_compile_time_expressions_526
 */ 
class MutatorFrontendAction_526 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(526)

private:
    class MutatorASTConsumer_526 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_526(Rewriter &R) : TheRewriter(R) {}
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

