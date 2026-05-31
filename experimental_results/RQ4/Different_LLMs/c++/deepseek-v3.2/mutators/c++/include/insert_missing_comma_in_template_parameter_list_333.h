//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Missing_Comma_In_Template_Parameter_List_333
 */ 
class MutatorFrontendAction_333 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(333)

private:
    class MutatorASTConsumer_333 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_333(Rewriter &R) : TheRewriter(R) {}
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

