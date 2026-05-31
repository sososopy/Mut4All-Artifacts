//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_structured_bindings_in_function_template_544
 */ 
class MutatorFrontendAction_544 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(544)

private:
    class MutatorASTConsumer_544 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_544(Rewriter &R) : TheRewriter(R) {}
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

