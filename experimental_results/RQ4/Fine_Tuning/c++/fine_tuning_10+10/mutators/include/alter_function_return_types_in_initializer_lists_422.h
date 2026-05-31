//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Function_Return_Types_In_Initializer_Lists_422
 */ 
class MutatorFrontendAction_422 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(422)

private:
    class MutatorASTConsumer_422 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_422(Rewriter &R) : TheRewriter(R) {}
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

