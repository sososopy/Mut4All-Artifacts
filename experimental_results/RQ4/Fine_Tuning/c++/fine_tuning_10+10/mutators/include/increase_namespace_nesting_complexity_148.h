//header file
#pragma once
#include "Mutator_base.h"

/**
 * Increase_Namespace_Nesting_Complexity_148
 */ 
class MutatorFrontendAction_148 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(148)

private:
    class MutatorASTConsumer_148 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_148(Rewriter &R) : TheRewriter(R) {}
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

