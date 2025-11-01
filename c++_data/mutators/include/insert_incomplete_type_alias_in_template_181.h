//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Incomplete_Type_Alias_In_Template_181
 */ 
class MutatorFrontendAction_181 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(181)

private:
    class MutatorASTConsumer_181 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_181(Rewriter &R) : TheRewriter(R) {}
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

