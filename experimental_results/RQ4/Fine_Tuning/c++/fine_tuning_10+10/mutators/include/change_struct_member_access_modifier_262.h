//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Struct_Member_Access_Modifier_262
 */ 
class MutatorFrontendAction_262 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(262)

private:
    class MutatorASTConsumer_262 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_262(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::CXXRecordDecl*, std::set<clang::AccessSpecifier>> classAccessSpecifiers;
    };
};

