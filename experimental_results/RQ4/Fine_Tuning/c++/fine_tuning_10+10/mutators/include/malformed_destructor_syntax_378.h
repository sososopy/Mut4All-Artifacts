//header file
#pragma once
#include "Mutator_base.h"

/**
 * Malformed_Destructor_Syntax_378
 */ 
class MutatorFrontendAction_378 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(378)

private:
    class MutatorASTConsumer_378 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_378(Rewriter &R) : TheRewriter(R) {}
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

