//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Null_Pointer_Dereference_243
 */ 
class MutatorFrontendAction_243 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(243)

private:
    class MutatorASTConsumer_243 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_243(Rewriter &R) : TheRewriter(R) {}
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

