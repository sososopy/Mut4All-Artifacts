//header file
#pragma once
#include "Mutator_base.h"

/**
 * enum_initialization_with_non_integral_expression_658
 */ 
class MutatorFrontendAction_658 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(658)

private:
    class MutatorASTConsumer_658 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_658(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

