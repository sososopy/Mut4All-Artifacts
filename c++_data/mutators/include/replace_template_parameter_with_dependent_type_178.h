//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_With_Dependent_Type_178
 */ 
class MutatorFrontendAction_178 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(178)

private:
    class MutatorASTConsumer_178 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_178(Rewriter &R) : TheRewriter(R) {}
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

