//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Array_Declaration_With_Negative_Size_349
 */ 
class MutatorFrontendAction_349 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(349)

private:
    class MutatorASTConsumer_349 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_349(Rewriter &R) : TheRewriter(R) {}
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

