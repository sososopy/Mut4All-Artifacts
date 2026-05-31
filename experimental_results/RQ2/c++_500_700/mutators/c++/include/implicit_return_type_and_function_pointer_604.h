//header file
#pragma once
#include "Mutator_base.h"

/**
 * implicit_return_type_and_function_pointer_604
 */ 
class MutatorFrontendAction_604 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(604)

private:
    class MutatorASTConsumer_604 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_604(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *TargetFunction = nullptr;
        const VarDecl *TargetVariable = nullptr;
    };
};

