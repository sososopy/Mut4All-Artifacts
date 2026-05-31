//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Access_With_Pointer_Member_Access_380
 */ 
class MutatorFrontendAction_380 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(380)

private:
    class MutatorASTConsumer_380 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_380(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

