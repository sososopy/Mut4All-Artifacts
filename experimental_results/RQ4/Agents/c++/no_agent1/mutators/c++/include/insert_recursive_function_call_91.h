//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Function_Call_91
 */ 
class MutatorFrontendAction_91 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(91)

private:
    class MutatorASTConsumer_91 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_91(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::FunctionDecl*> visitedFunctions;
    };
};

