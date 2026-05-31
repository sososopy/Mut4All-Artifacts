//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Constrained_Constructor_Template_127
 */ 
class MutatorFrontendAction_127 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(127)

private:
    class MutatorASTConsumer_127 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_127(Rewriter &R) : TheRewriter(R) {}
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

