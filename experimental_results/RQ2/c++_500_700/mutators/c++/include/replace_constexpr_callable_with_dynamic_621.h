//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_constexpr_callable_with_dynamic_621
 */ 
class MutatorFrontendAction_621 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(621)

private:
    class MutatorASTConsumer_621 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_621(Rewriter &R) : TheRewriter(R) {}
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

