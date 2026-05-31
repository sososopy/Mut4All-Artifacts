//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Return_Type_In_Function_Within_Struct_250
 */ 
class MutatorFrontendAction_250 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(250)

private:
    class MutatorASTConsumer_250 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_250(Rewriter &R) : TheRewriter(R) {}
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

