//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Template_Parameter_64
 */ 
class MutatorFrontendAction_64 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(64)

private:
    class MutatorASTConsumer_64 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_64(Rewriter &R) : TheRewriter(R) {}
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

