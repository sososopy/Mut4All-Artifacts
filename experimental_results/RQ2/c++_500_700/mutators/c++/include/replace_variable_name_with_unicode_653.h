//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_variable_name_with_unicode_653
 */ 
class MutatorFrontendAction_653 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(653)

private:
    class MutatorASTConsumer_653 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_653(Rewriter &R) : TheRewriter(R) {}
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

