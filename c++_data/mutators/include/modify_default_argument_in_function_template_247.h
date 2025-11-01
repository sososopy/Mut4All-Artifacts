//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Default_Argument_In_Function_Template_247
 */ 
class MutatorFrontendAction_247 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(247)

private:
    class MutatorASTConsumer_247 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_247(Rewriter &R) : TheRewriter(R) {}
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

