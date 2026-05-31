//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_class_member_instantiation_693
 */ 
class MutatorFrontendAction_693 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(693)

private:
    class MutatorASTConsumer_693 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_693(Rewriter &R) : TheRewriter(R) {}
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

