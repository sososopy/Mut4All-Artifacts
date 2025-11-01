//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Return_Type_With_Auto_123
 */ 
class MutatorFrontendAction_123 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(123)

private:
    class MutatorASTConsumer_123 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_123(Rewriter &R) : TheRewriter(R) {}
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

