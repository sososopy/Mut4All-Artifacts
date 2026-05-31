//header file
#pragma once
#include "Mutator_base.h"

/**
 * invalid_using_declaration_in_derived_class_654
 */ 
class MutatorFrontendAction_654 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(654)

private:
    class MutatorASTConsumer_654 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_654(Rewriter &R) : TheRewriter(R) {}
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

