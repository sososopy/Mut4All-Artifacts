//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misplaced_Preprocessor_Directives_361
 */ 
class MutatorFrontendAction_361 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(361)

private:
    class MutatorASTConsumer_361 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_361(Rewriter &R) : TheRewriter(R) {}
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

