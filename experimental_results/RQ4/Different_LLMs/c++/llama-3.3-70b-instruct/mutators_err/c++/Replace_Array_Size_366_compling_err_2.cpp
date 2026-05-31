```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Array_Size_366
 */ 
class MutatorFrontendAction_366 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(366)

private:
    class MutatorASTConsumer_366 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_366(Rewriter &R) : TheRewriter(R) {}
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

//source file
#include "../include/Replace_Array_Size_366.h"

// ========================================================================================================
#define MUT366_OUTPUT