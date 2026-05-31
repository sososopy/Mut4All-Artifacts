//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Atomic_Type_Comparison_Mismatch_237
 */ 
class MutatorFrontendAction_237 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(237)

private:
    class MutatorASTConsumer_237 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_237(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> atomic_vars;
    };
};

