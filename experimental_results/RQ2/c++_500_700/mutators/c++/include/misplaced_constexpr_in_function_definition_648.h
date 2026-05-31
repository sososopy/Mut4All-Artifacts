//header file
#pragma once
#include "Mutator_base.h"

/**
 * misplaced_constexpr_in_function_definition_648
 */ 
class MutatorFrontendAction_648 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(648)

private:
    class MutatorASTConsumer_648 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_648(Rewriter &R) : TheRewriter(R) {}
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

