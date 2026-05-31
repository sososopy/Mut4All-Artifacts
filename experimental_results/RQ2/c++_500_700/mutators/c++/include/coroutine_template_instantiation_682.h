//header file
#pragma once
#include "Mutator_base.h"

/**
 * coroutine_template_instantiation_682
 */ 
class MutatorFrontendAction_682 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(682)

private:
    class MutatorASTConsumer_682 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_682(Rewriter &R) : TheRewriter(R) {}
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

