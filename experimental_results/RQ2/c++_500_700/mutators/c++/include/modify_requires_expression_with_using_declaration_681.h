//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_requires_expression_with_using_declaration_681
 */ 
class MutatorFrontendAction_681 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(681)

private:
    class MutatorASTConsumer_681 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_681(Rewriter &R) : TheRewriter(R) {}
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

