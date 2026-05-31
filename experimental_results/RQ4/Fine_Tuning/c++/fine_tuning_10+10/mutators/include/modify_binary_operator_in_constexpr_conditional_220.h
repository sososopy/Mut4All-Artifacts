//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Binary_Operator_In_Constexpr_Conditional_220
 */ 
class MutatorFrontendAction_220 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(220)

private:
    class MutatorASTConsumer_220 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_220(Rewriter &R) : TheRewriter(R) {}
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

