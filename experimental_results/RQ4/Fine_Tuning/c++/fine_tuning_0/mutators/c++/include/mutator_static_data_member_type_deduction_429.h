//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Static_Data_Member_Type_Deduction_429
 */ 
class MutatorFrontendAction_429 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(429)

private:
    class MutatorASTConsumer_429 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_429(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TypeAliasDecl *type_alias_decl;
        const clang::CXXRecordDecl *record_decl;
        const clang::VarDecl *var_decl;
    };
};

