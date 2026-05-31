//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Explicit_Template_Args_To_Constexpr_Call_205
 */ 
class MutatorFrontendAction_205 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(205)

private:
    class MutatorASTConsumer_205 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_205(Rewriter &R) : TheRewriter(R) {}
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
    };
};

