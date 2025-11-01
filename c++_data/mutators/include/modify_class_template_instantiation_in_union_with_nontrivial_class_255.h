//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Class_Template_Instantiation_In_Union_With_Nontrivial_Class_255
 */ 
class MutatorFrontendAction_255 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(255)

private:
    class MutatorASTConsumer_255 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_255(Rewriter &R) : TheRewriter(R) {}
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

