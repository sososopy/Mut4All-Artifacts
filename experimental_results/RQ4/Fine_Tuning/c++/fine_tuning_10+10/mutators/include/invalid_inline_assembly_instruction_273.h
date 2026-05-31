//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Inline_Assembly_Instruction_273
 */ 
class MutatorFrontendAction_273 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(273)

private:
    class MutatorASTConsumer_273 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_273(Rewriter &R) : TheRewriter(R) {}
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

