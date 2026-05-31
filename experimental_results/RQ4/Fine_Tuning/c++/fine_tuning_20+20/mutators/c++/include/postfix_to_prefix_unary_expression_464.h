//header file
#pragma once
#include "Mutator_base.h"

/**
 * Postfix_To_Prefix_Unary_Expression_464
 */ 
class MutatorFrontendAction_464 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(464)

private:
    class MutatorASTConsumer_464 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_464(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

