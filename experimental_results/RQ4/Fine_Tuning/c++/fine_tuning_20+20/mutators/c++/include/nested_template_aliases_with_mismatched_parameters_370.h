//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Template_Aliases_With_Mismatched_Parameters_370
 */ 
class MutatorFrontendAction_370 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(370)

private:
    class MutatorASTConsumer_370 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_370(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> cur_templates;
    };
};

