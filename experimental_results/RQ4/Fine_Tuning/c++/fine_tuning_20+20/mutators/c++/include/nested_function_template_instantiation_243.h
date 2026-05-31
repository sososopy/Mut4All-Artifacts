//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Function_Template_Instantiation_243
 */ 
class MutatorFrontendAction_243 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(243)

private:
    class MutatorASTConsumer_243 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_243(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *last_struct = nullptr;
    };
};

