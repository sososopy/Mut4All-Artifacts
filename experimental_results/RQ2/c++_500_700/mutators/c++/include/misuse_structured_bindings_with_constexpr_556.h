//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_structured_bindings_with_constexpr_556
 */ 
class MutatorFrontendAction_556 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(556)

private:
    class MutatorASTConsumer_556 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_556(Rewriter &R) : TheRewriter(R) {}
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

