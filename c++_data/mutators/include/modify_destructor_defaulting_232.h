//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Destructor_Defaulting_232
 */ 
class MutatorFrontendAction_232 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(232)

private:
    class MutatorASTConsumer_232 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_232(Rewriter &R) : TheRewriter(R) {}
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

