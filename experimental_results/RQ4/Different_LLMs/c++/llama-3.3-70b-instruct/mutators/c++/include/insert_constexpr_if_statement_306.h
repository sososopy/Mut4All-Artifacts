//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Constexpr_If_Statement_306
 */ 
class MutatorFrontendAction_306 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(306)

private:
    class MutatorASTConsumer_306 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_306(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

