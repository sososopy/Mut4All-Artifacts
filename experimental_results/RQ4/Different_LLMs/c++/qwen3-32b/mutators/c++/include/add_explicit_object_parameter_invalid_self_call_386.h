//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Explicit_Object_Parameter_Invalid_Self_Call_386
 */ 
class MutatorFrontendAction_386 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(386)
private:
    class MutatorASTConsumer_386 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_386(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

