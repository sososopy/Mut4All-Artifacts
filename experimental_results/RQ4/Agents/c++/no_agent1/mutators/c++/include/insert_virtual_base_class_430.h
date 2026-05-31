//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_430
 */ 
class MutatorFrontendAction_430 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(430)

private:
    class MutatorASTConsumer_430 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_430(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> base_classes;
    };
};

