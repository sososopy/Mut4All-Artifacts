//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Template_Function_Signature_202
 */ 
class MutatorFrontendAction_202 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(202)

private:
    class MutatorASTConsumer_202 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_202(Rewriter &R) : TheRewriter(R) {}
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

