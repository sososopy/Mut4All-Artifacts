//header file
#pragma once
#include "Mutator_base.h"
#include <regex>

/**
 * Modify_Attributes_In_Lambda_Expressions_483
 */ 
class MutatorFrontendAction_483 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(483)

private:
    class MutatorASTConsumer_483 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_483(Rewriter &R) : TheRewriter(R) {}
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

