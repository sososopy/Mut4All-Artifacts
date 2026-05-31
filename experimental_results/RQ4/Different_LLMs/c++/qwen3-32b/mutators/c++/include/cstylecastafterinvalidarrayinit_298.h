//header file
#pragma once
#include "Mutator_base.h"

/**
 * CStyleCastAfterInvalidArrayInit_298
 */ 
class MutatorFrontendAction_298 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(298)

private:
    class MutatorASTConsumer_298 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_298(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
        std::vector<FunctionDecl*> functions;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, const std::vector<FunctionDecl*>& funcs) : Rewrite(Rewrite), functions(funcs) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        const std::vector<FunctionDecl*>& functions;
    };
};

