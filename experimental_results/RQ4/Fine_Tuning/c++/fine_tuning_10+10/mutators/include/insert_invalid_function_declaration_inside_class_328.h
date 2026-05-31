//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Function_Declaration_Inside_Class_328
 */ 
class MutatorFrontendAction_328 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(328)

private:
    class MutatorASTConsumer_328 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_328(Rewriter &R) : TheRewriter(R) {}
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

