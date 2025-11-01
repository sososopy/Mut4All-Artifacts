//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Parameter_With_Constraints_454
 */ 
class MutatorFrontendAction_454 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(454)

private:
    class MutatorASTConsumer_454 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_454(Rewriter &R) : TheRewriter(R) {}
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

