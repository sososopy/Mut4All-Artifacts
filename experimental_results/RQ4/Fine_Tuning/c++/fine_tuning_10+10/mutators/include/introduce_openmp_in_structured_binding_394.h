//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_OpenMP_In_Structured_Binding_394
 */ 
class MutatorFrontendAction_394 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(394)

private:
    class MutatorASTConsumer_394 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_394(Rewriter &R) : TheRewriter(R) {}
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
        const VarDecl *structuredBindingDecl = nullptr;
    };
};

