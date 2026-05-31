//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_CTAD_For_Alias_Template_With_Nested_Template_Classes_53
 */ 
class MutatorFrontendAction_53 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(53)

private:
    class MutatorASTConsumer_53 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_53(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
        const clang::FunctionDecl *MainFunc;
        std::vector<const clang::TypeAliasTemplateDecl *> AliasTemplates;
        std::vector<const clang::ClassTemplateDecl *> ClassTemplates;
    private:
        Rewriter &Rewrite;
    };
};

