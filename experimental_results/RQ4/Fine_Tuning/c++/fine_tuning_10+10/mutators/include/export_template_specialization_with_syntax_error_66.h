//header file
#pragma once
#include "Mutator_base.h"

/**
 * Export_Template_Specialization_With_Syntax_Error_66
 */ 
class MutatorFrontendAction_66 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(66)

private:
    class MutatorASTConsumer_66 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_66(Rewriter &R) : TheRewriter(R) {}
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

