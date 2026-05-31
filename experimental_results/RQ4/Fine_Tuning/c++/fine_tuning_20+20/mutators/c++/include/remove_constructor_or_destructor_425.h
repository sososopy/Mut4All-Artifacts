//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Constructor_Or_Destructor_425
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
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

