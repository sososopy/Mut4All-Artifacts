//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Attribute_With_Overloaded_Functions_443
 */ 
class MutatorFrontendAction_443 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(443)

private:
    class MutatorASTConsumer_443 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_443(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> overloadedFunctions;
    };
};

