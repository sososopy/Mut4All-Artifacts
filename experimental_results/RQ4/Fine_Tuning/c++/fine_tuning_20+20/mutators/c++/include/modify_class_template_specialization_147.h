//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Class_Template_Specialization_147
 */ 
class MutatorFrontendAction_147 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(147)

private:
    class MutatorASTConsumer_147 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_147(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> cur_templates;
        std::vector<const clang::ClassTemplatePartialSpecializationDecl *>
            cur_partials;
    };
};

