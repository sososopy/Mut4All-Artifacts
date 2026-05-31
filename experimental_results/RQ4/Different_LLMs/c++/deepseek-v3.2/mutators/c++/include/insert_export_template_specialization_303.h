//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Export_Template_Specialization_303
 */ 
class MutatorFrontendAction_303 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(303)

private:
    class MutatorASTConsumer_303 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_303(Rewriter &R) : TheRewriter(R) {}
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

