//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Inheritance_To_Composition_82
 */ 
class MutatorFrontendAction_82 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(82)

private:
    class MutatorASTConsumer_82 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_82(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *baseClassDecl = nullptr;
    };
};

