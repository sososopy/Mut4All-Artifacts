//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Function_Instantiation_Incorrect_Type_297
 */ 
class MutatorFrontendAction_297 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(297)

private:
    class MutatorASTConsumer_297 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_297(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const FunctionDecl*, std::string> templateFunctionCalls;
    };
};

