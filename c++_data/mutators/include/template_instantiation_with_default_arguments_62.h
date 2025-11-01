//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Instantiation_With_Default_Arguments_62
 */ 
class MutatorFrontendAction_62 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(62)

private:
    class MutatorASTConsumer_62 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_62(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> templates;
    };
};

