//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Null_Pointer_Dereference_203
 */ 
class MutatorFrontendAction_203 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(203)

private:
    class MutatorASTConsumer_203 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_203(Rewriter &R) : TheRewriter(R) {}
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

