//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Namespace_Template_Specialization_5
 */ 
class MutatorFrontendAction_5 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(5)

private:
    class MutatorASTConsumer_5 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_5(Rewriter &R) : TheRewriter(R) {}
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

