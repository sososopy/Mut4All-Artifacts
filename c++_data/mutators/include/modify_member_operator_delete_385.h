//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Member_Operator_Delete_385
 */ 
class MutatorFrontendAction_385 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(385)

private:
    class MutatorASTConsumer_385 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_385(Rewriter &R) : TheRewriter(R) {}
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

