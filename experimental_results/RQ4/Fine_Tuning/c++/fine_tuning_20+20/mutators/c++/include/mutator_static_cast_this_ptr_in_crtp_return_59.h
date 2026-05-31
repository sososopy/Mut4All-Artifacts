//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Static_Cast_This_Ptr_In_Crtp_Return_59
 */ 
class MutatorFrontendAction_59 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(59)

private:
    class MutatorASTConsumer_59 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_59(Rewriter &R) : TheRewriter(R) {}
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

