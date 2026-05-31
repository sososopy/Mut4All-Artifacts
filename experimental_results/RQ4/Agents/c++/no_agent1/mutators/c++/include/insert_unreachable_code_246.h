//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_246
 */ 
class MutatorFrontendAction_246 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(246)

private:
    class MutatorASTConsumer_246 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_246(Rewriter &R) : TheRewriter(R) {}
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

