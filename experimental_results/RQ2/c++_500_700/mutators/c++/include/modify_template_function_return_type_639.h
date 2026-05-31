//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_function_return_type_639
 */ 
class MutatorFrontendAction_639 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(639)

private:
    class MutatorASTConsumer_639 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_639(Rewriter &R) : TheRewriter(R) {}
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

