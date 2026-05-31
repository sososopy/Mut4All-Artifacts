//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Operator_With_Function_Call_464
 */ 

class MutatorFrontendAction_464 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(464)
private:
    class MutatorASTConsumer_464 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_464(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::string> operatorToFunctionMap;
    };
};

