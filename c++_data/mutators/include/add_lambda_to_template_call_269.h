//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Lambda_To_Template_Call_269
 */ 
class MutatorFrontendAction_269 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(269)

private:
    class MutatorASTConsumer_269 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_269(Rewriter &R) : TheRewriter(R) {}
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

