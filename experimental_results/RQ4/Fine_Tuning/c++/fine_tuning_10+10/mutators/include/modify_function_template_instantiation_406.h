//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Template_Instantiation_406
 */ 
class MutatorFrontendAction_406 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(406)

private:
    class MutatorASTConsumer_406 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_406(Rewriter &R) : TheRewriter(R) {}
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
    };
};

