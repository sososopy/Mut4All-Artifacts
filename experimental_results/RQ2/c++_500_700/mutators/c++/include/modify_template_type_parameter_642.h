//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_type_parameter_642
 */ 
class MutatorFrontendAction_642 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(642)

private:
    class MutatorASTConsumer_642 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_642(Rewriter &R) : TheRewriter(R) {}
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

