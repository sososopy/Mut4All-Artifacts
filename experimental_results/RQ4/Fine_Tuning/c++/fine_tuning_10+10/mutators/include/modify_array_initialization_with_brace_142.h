//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Array_Initialization_With_Brace_142
 */ 
class MutatorFrontendAction_142 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(142)

private:
    class MutatorASTConsumer_142 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_142(Rewriter &R) : TheRewriter(R) {}
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

