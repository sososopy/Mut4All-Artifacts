//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_consteval_function_invocation_638
 */ 
class MutatorFrontendAction_638 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(638)

private:
    class MutatorASTConsumer_638 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_638(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const FunctionDecl*, std::string> constevalFunctions;
    };
};

