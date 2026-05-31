//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Requirements_316
 */ 
class MutatorFrontendAction_316 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(316)

private:
    class MutatorASTConsumer_316 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_316(Rewriter &R) : TheRewriter(R) {}
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

