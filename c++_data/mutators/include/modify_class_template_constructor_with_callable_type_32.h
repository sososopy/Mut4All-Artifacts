//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Class_Template_Constructor_With_Callable_Type_32
 */ 
class MutatorFrontendAction_32 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(32)

private:
    class MutatorASTConsumer_32 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_32(Rewriter &R) : TheRewriter(R) {}
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
        const CXXConstructorDecl *ctorDecl = nullptr;
    };
};

