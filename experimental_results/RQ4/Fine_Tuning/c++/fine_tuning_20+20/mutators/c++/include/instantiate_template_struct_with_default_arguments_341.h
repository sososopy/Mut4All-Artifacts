//header file
#pragma once
#include "Mutator_base.h"

/**
 * Instantiate_Template_Struct_With_Default_Arguments_341
 */ 
class MutatorFrontendAction_341 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(341)

private:
    class MutatorASTConsumer_341 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_341(Rewriter &R) : TheRewriter(R) {}
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

