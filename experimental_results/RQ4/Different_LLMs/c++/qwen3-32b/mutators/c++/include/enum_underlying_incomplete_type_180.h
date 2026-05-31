//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Underlying_Incomplete_Type_180
 */ 
class MutatorFrontendAction_180 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(180)

private:
    class MutatorASTConsumer_180 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_180(Rewriter &R) : TheRewriter(R) {}
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
        static int counter;
    };
};

