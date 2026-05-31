//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_function_parameter_504
 */ 
class MutatorFrontendAction_504 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(504)

private:
    class MutatorASTConsumer_504 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_504(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

