//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constexpr_With_Non_Constant_Expression_40
 */ 
class MutatorFrontendAction_40 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(40)

private:
    class MutatorASTConsumer_40 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_40(Rewriter &R) : TheRewriter(R) {}
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

