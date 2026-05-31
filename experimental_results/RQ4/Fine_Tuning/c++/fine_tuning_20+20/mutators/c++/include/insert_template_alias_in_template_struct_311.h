//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Template_Alias_In_Template_Struct_311
 */ 
class MutatorFrontendAction_311 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(311)

private:
    class MutatorASTConsumer_311 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_311(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_structs;
    };
};

