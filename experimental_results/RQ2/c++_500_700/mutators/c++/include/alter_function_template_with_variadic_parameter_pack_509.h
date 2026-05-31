//header file
#pragma once
#include "Mutator_base.h"

/**
 * alter_function_template_with_variadic_parameter_pack_509
 */ 
class MutatorFrontendAction_509 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(509)

private:
    class MutatorASTConsumer_509 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_509(Rewriter &R) : TheRewriter(R) {}
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

