//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Function_Return_Type_374
 */ 
class MutatorFrontendAction_374 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(374)

private:
    class MutatorASTConsumer_374 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_374(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *TargetFuncDecl;
    };
};

