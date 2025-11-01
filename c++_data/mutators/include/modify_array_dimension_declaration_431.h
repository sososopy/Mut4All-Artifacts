//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Array_Dimension_Declaration_431
 */ 
class MutatorFrontendAction_431 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(431)

private:
    class MutatorASTConsumer_431 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_431(Rewriter &R) : TheRewriter(R) {}
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

