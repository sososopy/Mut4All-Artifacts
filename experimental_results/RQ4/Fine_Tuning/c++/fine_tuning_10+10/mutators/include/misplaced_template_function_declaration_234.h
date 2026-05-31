//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misplaced_Template_Function_Declaration_234
 */ 
class MutatorFrontendAction_234 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(234)

private:
    class MutatorASTConsumer_234 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_234(Rewriter &R) : TheRewriter(R) {}
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
        const CXXRecordDecl *currentClass = nullptr;
    };
};

