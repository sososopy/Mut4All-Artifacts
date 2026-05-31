//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Default_Argument_Initializer_212
 */ 
class MutatorFrontendAction_212 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(212)

private:
    class MutatorASTConsumer_212 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_212(Rewriter &R) : TheRewriter(R) {}
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

