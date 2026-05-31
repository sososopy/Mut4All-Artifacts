//header file
#pragma once
#include "Mutator_base.h"

/**
 * Union_Initializer_Mutation_221
 */ 
class MutatorFrontendAction_221 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(221)

private:
    class MutatorASTConsumer_221 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_221(Rewriter &R) : TheRewriter(R) {}
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

