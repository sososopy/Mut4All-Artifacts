//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Inheritance_And_Access_Specifiers_467
 */ 
class MutatorFrontendAction_467 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(467)

private:
    class MutatorASTConsumer_467 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_467(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> derivedStructs;
    };
};

