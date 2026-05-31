//header file
#pragma once
#include "Mutator_base.h"

/**
 * remove_template_type_qualifier_632
 */ 
class MutatorFrontendAction_632 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(632)

private:
    class MutatorASTConsumer_632 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_632(Rewriter &R) : TheRewriter(R) {}
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

