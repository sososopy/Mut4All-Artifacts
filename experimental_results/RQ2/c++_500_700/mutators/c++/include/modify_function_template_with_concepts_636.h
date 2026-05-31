//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_function_template_with_concepts_636
 */ 
class MutatorFrontendAction_636 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(636)

private:
    class MutatorASTConsumer_636 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_636(Rewriter &R) : TheRewriter(R) {}
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

