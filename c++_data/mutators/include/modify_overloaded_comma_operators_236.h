//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Overloaded_Comma_Operators_236
 */ 
class MutatorFrontendAction_236 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(236)

private:
    class MutatorASTConsumer_236 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_236(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *CurrentStruct = nullptr;
    };
};

