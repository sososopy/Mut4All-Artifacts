//header file
#pragma once
#include "Mutator_base.h"

/**
 * extra_qualifier_on_member_function_629
 */ 
class MutatorFrontendAction_629 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(629)

private:
    class MutatorASTConsumer_629 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_629(Rewriter &R) : TheRewriter(R) {}
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

