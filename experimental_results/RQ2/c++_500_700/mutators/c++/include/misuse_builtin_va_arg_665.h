//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_builtin_va_arg_665
 */ 
class MutatorFrontendAction_665 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(665)

private:
    class MutatorASTConsumer_665 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_665(Rewriter &R) : TheRewriter(R) {}
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
        const clang::VarDecl *misusedVar = nullptr;
    };
};

