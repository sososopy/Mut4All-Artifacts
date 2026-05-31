//header file
#pragma once
#include "Mutator_base.h"

/**
 * Duplicate_Or_Insert_Template_Specialization_74
 */ 
class MutatorFrontendAction_74 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(74)

private:
    class MutatorASTConsumer_74 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_74(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
        std::vector<const clang::ClassTemplateDecl *> cur_class_templates;
        std::vector<const clang::FunctionTemplateDecl *> cur_function_templates;
        std::vector<const clang::ClassTemplateSpecializationDecl *> cur_class_specializations;
        std::vector<const clang::FunctionDecl *> cur_function_specializations;
        std::vector<int> cur_class_specialization_type;
        std::vector<int> cur_function_specialization_type;
    private:
        Rewriter &Rewrite;
    };
};

