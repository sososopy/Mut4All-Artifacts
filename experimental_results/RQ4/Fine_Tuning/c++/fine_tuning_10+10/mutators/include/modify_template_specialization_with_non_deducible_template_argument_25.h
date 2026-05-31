//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Specialization_With_Non_deducible_Template_Argument_25
 */ 
class MutatorFrontendAction_25 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(25)

private:
    class MutatorASTConsumer_25 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_25(Rewriter &R) : TheRewriter(R) {}
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

