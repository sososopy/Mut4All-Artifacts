//header file
#pragma once
#include "Mutator_base.h"

/**
 * Initialize_Std_Array_With_Element_418
 */ 
class MutatorFrontendAction_418 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(418)

private:
    class MutatorASTConsumer_418 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_418(Rewriter &R) : TheRewriter(R) {}
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

