//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Typedef_To_Function_Declaration_416
 */ 
class MutatorFrontendAction_416 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(416)

private:
    class MutatorASTConsumer_416 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_416(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::FunctionDecl*, std::string> typedefMap;
    };
};

