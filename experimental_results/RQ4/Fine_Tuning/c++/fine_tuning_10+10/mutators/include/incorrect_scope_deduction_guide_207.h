//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incorrect_Scope_Deduction_Guide_207
 */ 
class MutatorFrontendAction_207 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(207)

private:
    class MutatorASTConsumer_207 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_207(Rewriter &R) : TheRewriter(R) {}
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
        std::unordered_set<const clang::NamespaceDecl *> visitedNamespaces;
    };
};

