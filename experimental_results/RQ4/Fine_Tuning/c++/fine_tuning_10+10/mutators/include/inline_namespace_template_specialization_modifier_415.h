//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Namespace_Template_Specialization_Modifier_415
 */ 
class MutatorFrontendAction_415 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(415)

private:
    class MutatorASTConsumer_415 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_415(Rewriter &R) : TheRewriter(R) {}
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
        const NamespaceDecl *inlineNamespace = nullptr;
        const FunctionTemplateDecl *templateDecl = nullptr;
    };
};

