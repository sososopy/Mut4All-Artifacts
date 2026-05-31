//header file
#pragma once
#include "Mutator_base.h"

/**
 * insert_incomplete_struct_initialization_502
 */ 
class MutatorFrontendAction_502 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(502)

private:
    class MutatorASTConsumer_502 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_502(Rewriter &R) : TheRewriter(R) {}
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

