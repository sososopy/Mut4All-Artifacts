//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Arbitrary_Identifier_Before_Constructor_354
 */ 
class MutatorFrontendAction_354 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(354)

private:
    class MutatorASTConsumer_354 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_354(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

