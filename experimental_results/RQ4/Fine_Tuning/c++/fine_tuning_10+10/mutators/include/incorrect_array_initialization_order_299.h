//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incorrect_Array_Initialization_Order_299
 */ 
class MutatorFrontendAction_299 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(299)

private:
    class MutatorASTConsumer_299 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_299(Rewriter &R) : TheRewriter(R) {}
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
        const VarDecl *staticVarDecl = nullptr;
    };
};

