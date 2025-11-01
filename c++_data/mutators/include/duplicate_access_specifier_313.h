//header file
#pragma once
#include "Mutator_base.h"

/**
 * Duplicate_Access_Specifier_313
 */ 
class MutatorFrontendAction_313 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(313)

private:
    class MutatorASTConsumer_313 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_313(Rewriter &R) : TheRewriter(R) {}
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

