//header file
#pragma once
#include "Mutator_base.h"

/**
 * inline_namespace_class_declaration_mutation_501
 */ 
class MutatorFrontendAction_501 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(501)

private:
    class MutatorASTConsumer_501 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_501(Rewriter &R) : TheRewriter(R) {}
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

