//header file
#pragma once
#include "Mutator_base.h"

/**
 * Overload_Function_Template_Specialization_304
 */ 
class MutatorFrontendAction_304 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(304)

private:
    class MutatorASTConsumer_304 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_304(Rewriter &R) : TheRewriter(R) {}
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

