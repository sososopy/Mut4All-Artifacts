//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Lambda_With_Pack_Expansion_In_Requires_Clause_117
 */ 
class MutatorFrontendAction_117 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(117)

private:
    class MutatorASTConsumer_117 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_117(Rewriter &R) : TheRewriter(R) {}
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

