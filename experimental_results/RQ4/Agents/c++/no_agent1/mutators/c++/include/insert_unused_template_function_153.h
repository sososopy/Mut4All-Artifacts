//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Function_153
 */ 
class MutatorFrontendAction_153 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(153)

private:
    class MutatorASTConsumer_153 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_153(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::FunctionDecl *> processedFunctions;
    };
};

