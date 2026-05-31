//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_function_pointer_dereference_625
 */ 
class MutatorFrontendAction_625 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(625)

private:
    class MutatorASTConsumer_625 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_625(Rewriter &R) : TheRewriter(R) {}
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

