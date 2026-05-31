//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_type_traits_variable_declarations_672
 */ 
class MutatorFrontendAction_672 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(672)

private:
    class MutatorASTConsumer_672 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_672(Rewriter &R) : TheRewriter(R) {}
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

