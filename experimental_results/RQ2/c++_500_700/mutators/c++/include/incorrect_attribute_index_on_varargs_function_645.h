//header file
#pragma once
#include "Mutator_base.h"

/**
 * incorrect_attribute_index_on_varargs_function_645
 */ 
class MutatorFrontendAction_645 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(645)

private:
    class MutatorASTConsumer_645 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_645(Rewriter &R) : TheRewriter(R) {}
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

