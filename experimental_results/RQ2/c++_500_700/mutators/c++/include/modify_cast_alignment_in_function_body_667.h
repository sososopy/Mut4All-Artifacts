//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_cast_alignment_in_function_body_667
 */ 
class MutatorFrontendAction_667 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(667)

private:
    class MutatorASTConsumer_667 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_667(Rewriter &R) : TheRewriter(R) {}
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

