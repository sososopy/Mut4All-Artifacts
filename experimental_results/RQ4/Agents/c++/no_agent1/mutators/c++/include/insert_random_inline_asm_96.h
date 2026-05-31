//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Random_Inline_Asm_96
 */ 
class MutatorFrontendAction_96 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(96)
private:
    class MutatorASTConsumer_96 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_96(Rewriter &R) : TheRewriter(R) {}
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

