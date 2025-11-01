//header file
#pragma once
#include "Mutator_base.h"

/**
 * Uninitialized_Member_Access_335
 */ 
class MutatorFrontendAction_335 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(335)

private:
    class MutatorASTConsumer_335 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_335(Rewriter &R) : TheRewriter(R) {}
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
        const FieldDecl *TargetField = nullptr;
    };
};

