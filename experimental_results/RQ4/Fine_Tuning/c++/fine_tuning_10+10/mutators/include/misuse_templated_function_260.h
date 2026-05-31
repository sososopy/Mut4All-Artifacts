//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misuse_Templated_Function_260
 */ 
class MutatorFrontendAction_260 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(260)

private:
    class MutatorASTConsumer_260 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_260(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionTemplateDecl *TargetTemplate = nullptr;
    };
};

