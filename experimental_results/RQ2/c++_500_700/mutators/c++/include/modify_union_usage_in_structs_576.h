//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_union_usage_in_structs_576
 */ 
class MutatorFrontendAction_576 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(576)

private:
    class MutatorASTConsumer_576 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_576(Rewriter &R) : TheRewriter(R) {}
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

