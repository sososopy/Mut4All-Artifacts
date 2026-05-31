//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constexpr_With_Inline_In_Deleted_Member_Function_15
 */ 
class MutatorFrontendAction_15 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(15)

private:
    class MutatorASTConsumer_15 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_15(Rewriter &R) : TheRewriter(R) {}
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

