//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Incomplete_Type_With_Member_Pointer_258
 */ 
class MutatorFrontendAction_258 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(258)

private:
    class MutatorASTConsumer_258 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_258(Rewriter &R) : TheRewriter(R) {}
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
    };
};

