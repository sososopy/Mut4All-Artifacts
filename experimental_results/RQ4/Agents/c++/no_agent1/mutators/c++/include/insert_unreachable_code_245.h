//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_245
 */ 
class MutatorFrontendAction_245 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(245)

private:
    class MutatorASTConsumer_245 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_245(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> functions;
    };
};

