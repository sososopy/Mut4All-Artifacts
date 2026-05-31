//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_If_Statement_192
 */ 
class MutatorFrontendAction_192 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(192)

private:
    class MutatorASTConsumer_192 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_192(Rewriter &R) : TheRewriter(R) {}
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

