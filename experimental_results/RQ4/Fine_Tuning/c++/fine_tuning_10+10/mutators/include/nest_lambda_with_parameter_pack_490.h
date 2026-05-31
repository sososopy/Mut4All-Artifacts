//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nest_Lambda_With_Parameter_Pack_490
 */ 
class MutatorFrontendAction_490 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(490)

private:
    class MutatorASTConsumer_490 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_490(Rewriter &R) : TheRewriter(R) {}
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

