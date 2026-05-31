//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_argument_to_incomplete_type_679
 */ 
class MutatorFrontendAction_679 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(679)

private:
    class MutatorASTConsumer_679 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_679(Rewriter &R) : TheRewriter(R) {}
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

