//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Constrained_Template_Class_459
 */ 
class MutatorFrontendAction_459 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(459)

private:
    class MutatorASTConsumer_459 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_459(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<const clang::TemplateDecl *> cur_templates;
        std::vector<const clang::TypeAliasDecl *> cur_alias;
    };
};

