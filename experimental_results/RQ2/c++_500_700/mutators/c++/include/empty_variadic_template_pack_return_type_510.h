//header file
#pragma once
#include "Mutator_base.h"

/**
 * empty_variadic_template_pack_return_type_510
 */ 
class MutatorFrontendAction_510 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(510)

private:
    class MutatorASTConsumer_510 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_510(Rewriter &R) : TheRewriter(R) {}
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

