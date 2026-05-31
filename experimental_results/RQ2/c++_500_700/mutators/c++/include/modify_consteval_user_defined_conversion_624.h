//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_consteval_user_defined_conversion_624
 */ 
class MutatorFrontendAction_624 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(624)

private:
    class MutatorASTConsumer_624 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_624(Rewriter &R) : TheRewriter(R) {}
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

