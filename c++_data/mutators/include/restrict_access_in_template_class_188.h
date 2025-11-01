//header file
#pragma once
#include "Mutator_base.h"

/**
 * Restrict_Access_In_Template_Class_188
 */ 
class MutatorFrontendAction_188 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(188)

private:
    class MutatorASTConsumer_188 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_188(Rewriter &R) : TheRewriter(R) {}
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

