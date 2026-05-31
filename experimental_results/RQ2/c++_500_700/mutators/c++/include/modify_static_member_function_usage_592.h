//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_static_member_function_usage_592
 */ 
class MutatorFrontendAction_592 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(592)

private:
    class MutatorASTConsumer_592 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_592(Rewriter &R) : TheRewriter(R) {}
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

