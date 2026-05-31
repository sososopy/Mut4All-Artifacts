//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Incomplete_Union_Struct_263
 */ 
class MutatorFrontendAction_263 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(263)

private:
    class MutatorASTConsumer_263 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_263(Rewriter &R) : TheRewriter(R) {}
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

