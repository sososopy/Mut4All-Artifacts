//header file
#pragma once
#include "Mutator_base.h"

/**
 * misaligned_struct_declaration_567
 */ 
class MutatorFrontendAction_567 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(567)

private:
    class MutatorASTConsumer_567 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_567(Rewriter &R) : TheRewriter(R) {}
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

