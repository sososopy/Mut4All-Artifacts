//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceLambdaWithFunctionPointer_183
 */ 
class MutatorFrontendAction_183 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(183)

private:
    class MutatorASTConsumer_183 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_183(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

