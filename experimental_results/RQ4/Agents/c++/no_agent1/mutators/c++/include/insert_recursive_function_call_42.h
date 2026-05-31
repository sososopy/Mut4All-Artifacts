//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Function_Call_42
 */ 
class MutatorFrontendAction_42 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(42)

private:
    class MutatorASTConsumer_42 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_42(Rewriter &R) : TheRewriter(R) {}
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
        const clang::FunctionDecl *TargetFunction = nullptr;
    };
};

