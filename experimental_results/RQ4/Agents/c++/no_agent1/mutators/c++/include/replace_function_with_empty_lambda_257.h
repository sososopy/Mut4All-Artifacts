//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_With_Empty_Lambda_257
 */ 
class MutatorFrontendAction_257 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(257)

private:
    class MutatorASTConsumer_257 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_257(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> targetFunctions;
    };
};

