//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Method_With_Explicit_Object_Parameter_405
 */ 
class MutatorFrontendAction_405 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(405)

private:
    class MutatorASTConsumer_405 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_405(Rewriter &R) : TheRewriter(R) {}
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

