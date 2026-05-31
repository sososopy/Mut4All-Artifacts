//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Class_Nested_Interaction_261
 */ 
class MutatorFrontendAction_261 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(261)

private:
    class MutatorASTConsumer_261 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_261(Rewriter &R) : TheRewriter(R) {}
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

