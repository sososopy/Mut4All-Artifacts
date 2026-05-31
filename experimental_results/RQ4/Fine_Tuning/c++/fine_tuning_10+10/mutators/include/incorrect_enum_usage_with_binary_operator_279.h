//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incorrect_Enum_Usage_With_Binary_Operator_279
 */ 
class MutatorFrontendAction_279 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(279)

private:
    class MutatorASTConsumer_279 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_279(Rewriter &R) : TheRewriter(R) {}
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

