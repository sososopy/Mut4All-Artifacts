//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Constexpr_Deleted_Assignment_318
 */ 
class MutatorFrontendAction_318 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(318)

private:
    class MutatorASTConsumer_318 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_318(Rewriter &R) : TheRewriter(R) {}
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

