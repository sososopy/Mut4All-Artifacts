//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Integer_Literal_352
 */ 
class MutatorFrontendAction_352 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(352)

private:
    class MutatorASTConsumer_352 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_352(Rewriter &R) : TheRewriter(R) {}
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

