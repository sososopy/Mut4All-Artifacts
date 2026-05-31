//header file
#pragma once
#include "Mutator_base.h"

/**
 * lambda_expression_attribute_syntax_error_594
 */ 
class MutatorFrontendAction_594 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(594)

private:
    class MutatorASTConsumer_594 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_594(Rewriter &R) : TheRewriter(R) {}
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

