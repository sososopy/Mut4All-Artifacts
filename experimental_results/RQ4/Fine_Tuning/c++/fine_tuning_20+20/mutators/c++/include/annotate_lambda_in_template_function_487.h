//header file
#pragma once
#include "Mutator_base.h"

/**
 * Annotate_Lambda_In_Template_Function_487
 */ 
class MutatorFrontendAction_487 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(487)

private:
    class MutatorASTConsumer_487 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_487(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> cur_templates;
    };
};

