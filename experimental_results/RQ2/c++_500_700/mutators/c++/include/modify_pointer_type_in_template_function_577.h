//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_pointer_type_in_template_function_577
 */ 
class MutatorFrontendAction_577 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(577)

private:
    class MutatorASTConsumer_577 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_577(Rewriter &R) : TheRewriter(R) {}
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

