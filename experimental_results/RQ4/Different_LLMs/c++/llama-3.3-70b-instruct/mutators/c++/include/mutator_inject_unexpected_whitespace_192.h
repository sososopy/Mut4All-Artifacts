//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Unexpected_Whitespace_192
 */ 
class MutatorFrontendAction_192 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(192)

private:
    class MutatorASTConsumer_192 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_192(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

