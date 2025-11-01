//header file
#pragma once
#include "Mutator_base.h"

/**
 * Deep_Nested_Const_Reference_308
 */ 
class MutatorFrontendAction_308 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(308)

private:
    class MutatorASTConsumer_308 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_308(Rewriter &R) : TheRewriter(R) {}
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

