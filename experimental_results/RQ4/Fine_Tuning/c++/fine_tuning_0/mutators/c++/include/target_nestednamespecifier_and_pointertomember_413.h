//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_NestedNameSpecifier_And_PointerToMember_413
 */ 
class MutatorFrontendAction_413 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(413)

private:
    class MutatorASTConsumer_413 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_413(Rewriter &R) : TheRewriter(R) {}
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
        const clang::NamespaceDecl *cur_ns;
        const clang::CXXRecordDecl *cur_class;
        const clang::CXXRecordDecl *cur_struct;
        const clang::CXXMethodDecl *cur_method;
        const clang::VarDecl *cur_var;
    };
};

