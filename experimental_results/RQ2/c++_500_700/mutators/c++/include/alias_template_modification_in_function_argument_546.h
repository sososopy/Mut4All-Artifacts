//header file
#pragma once
#include "Mutator_base.h"

/**
 * alias_template_modification_in_function_argument_546
 */ 
class MutatorFrontendAction_546 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(546)

private:
    class MutatorASTConsumer_546 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_546(Rewriter &R) : TheRewriter(R) {}
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

