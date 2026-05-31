//header file
#pragma once
#include "Mutator_base.h"
#include <type_traits>

/**
 * modify_template_exception_specification_591
 */ 
class MutatorFrontendAction_591 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(591)

private:
    class MutatorASTConsumer_591 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_591(Rewriter &R) : TheRewriter(R) {}
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

