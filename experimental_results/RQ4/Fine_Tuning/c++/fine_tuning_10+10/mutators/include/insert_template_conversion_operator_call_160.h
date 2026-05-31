//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Template_Conversion_Operator_Call_160
 */ 
class MutatorFrontendAction_160 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(160)

private:
    class MutatorASTConsumer_160 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_160(Rewriter &R) : TheRewriter(R) {}
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

