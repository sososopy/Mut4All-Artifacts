//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Nested_Namespace_283
 */ 
class MutatorFrontendAction_283 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(283)

private:
    class MutatorASTConsumer_283 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_283(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamespaceDecl *> nested_namespaces;
    };
};

