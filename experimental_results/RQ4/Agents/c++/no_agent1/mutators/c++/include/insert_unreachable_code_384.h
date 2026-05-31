//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_384
 */ 
class MutatorFrontendAction_384 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(384)

private:
    class MutatorASTConsumer_384 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_384(Rewriter &R) : TheRewriter(R) {}
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

