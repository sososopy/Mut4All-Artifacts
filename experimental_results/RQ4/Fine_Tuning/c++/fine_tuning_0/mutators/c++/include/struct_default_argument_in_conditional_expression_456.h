//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Default_Argument_in_Conditional_Expression_456
 */ 
class MutatorFrontendAction_456 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(456)

private:
    class MutatorASTConsumer_456 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_456(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *target_struct = nullptr;
    };
};

