//header file
#pragma once
#include "Mutator_base.h"
#include <regex>
/**
 * Enhance_Template_Recursion_156
 */ 
class MutatorFrontendAction_156 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(156)

private:
    class MutatorASTConsumer_156 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_156(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::FunctionTemplateDecl *> cur_templates;
    };
};

