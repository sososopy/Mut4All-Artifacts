//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Unreachable_Code_110
 */ 

class MutatorFrontendAction_110 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(110)
private:
    class MutatorASTConsumer_110 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_110(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> targetFunctions;
    };
};

