//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Parameter_Pack_To_Lambda_58
 */ 
class MutatorFrontendAction_58 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(58)

private:
    class MutatorASTConsumer_58 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_58(Rewriter &R) : TheRewriter(R) {}
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

