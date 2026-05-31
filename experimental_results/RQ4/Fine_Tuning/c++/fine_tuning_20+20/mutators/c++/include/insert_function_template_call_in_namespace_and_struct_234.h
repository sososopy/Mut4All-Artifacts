//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Function_Template_Call_In_Namespace_And_Struct_234
 */ 
class MutatorFrontendAction_234 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(234)

private:
    class MutatorASTConsumer_234 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_234(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
        const clang::NamespaceDecl *target_ns;
        const clang::FunctionTemplateDecl *target_ft;
        const clang::CXXRecordDecl *target_st;
    private:
        Rewriter &Rewrite;
    };
};

