//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Return_Type_To_Pointer_298
 */ 
class MutatorFrontendAction_298 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(298)

private:
    class MutatorASTConsumer_298 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_298(Rewriter &R) : TheRewriter(R) {}
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

