//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Destructor_To_Pure_Virtual_18
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)

private:
    class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_18(Rewriter &R) : TheRewriter(R) {}
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

