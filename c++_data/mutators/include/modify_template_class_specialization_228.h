//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Class_Specialization_228
 */ 
class MutatorFrontendAction_228 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(228)

private:
    class MutatorASTConsumer_228 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_228(Rewriter &R) : TheRewriter(R) {}
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

