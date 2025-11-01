//header file
#pragma once
#include "Mutator_base.h"

/**
 * Static_Variable_Incrementation_In_Consteval_325
 */ 
class MutatorFrontendAction_325 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(325)

private:
    class MutatorASTConsumer_325 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_325(Rewriter &R) : TheRewriter(R) {}
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

