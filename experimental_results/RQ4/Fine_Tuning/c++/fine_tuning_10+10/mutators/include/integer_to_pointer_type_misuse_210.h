//header file
#pragma once
#include "Mutator_base.h"

/**
 * Integer_To_Pointer_Type_Misuse_210
 */ 
class MutatorFrontendAction_210 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(210)

private:
    class MutatorASTConsumer_210 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_210(Rewriter &R) : TheRewriter(R) {}
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

