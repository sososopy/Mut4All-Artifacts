//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Nested_Template_Requirements_70
 */ 
class MutatorFrontendAction_70 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(70)

private:
    class MutatorASTConsumer_70 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_70(Rewriter &R) : TheRewriter(R) {}
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

