//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Template_With_Default_Args_425
 */ 
class MutatorFrontendAction_425 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(425)

private:
    class MutatorASTConsumer_425 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_425(Rewriter &R) : TheRewriter(R) {}
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

