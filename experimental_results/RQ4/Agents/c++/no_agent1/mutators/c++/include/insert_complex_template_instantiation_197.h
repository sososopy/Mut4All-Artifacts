//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Complex_Template_Instantiation_197
 */ 
class MutatorFrontendAction_197 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(197)

private:
    class MutatorASTConsumer_197 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_197(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> templateClasses;
    };
};

