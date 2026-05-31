//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Constexpr_Expression_189
 */ 
class MutatorFrontendAction_189 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(189)

private:
    class MutatorASTConsumer_189 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_189(Rewriter &R) : TheRewriter(R) {}
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

