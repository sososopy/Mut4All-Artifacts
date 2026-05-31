//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_function_declaration_return_type_569
 */ 
class MutatorFrontendAction_569 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(569)

private:
    class MutatorASTConsumer_569 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_569(Rewriter &R) : TheRewriter(R) {}
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

