//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Struct_Destructor_Call_248
 */ 
class MutatorFrontendAction_248 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(248)

private:
    class MutatorASTConsumer_248 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_248(Rewriter &R) : TheRewriter(R) {}
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

