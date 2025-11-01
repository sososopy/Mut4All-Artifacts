//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Consteval_Constructor_69
 */ 
class MutatorFrontendAction_69 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(69)

private:
    class MutatorASTConsumer_69 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_69(Rewriter &R) : TheRewriter(R) {}
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

