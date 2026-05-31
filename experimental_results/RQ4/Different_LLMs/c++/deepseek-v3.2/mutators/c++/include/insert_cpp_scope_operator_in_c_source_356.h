//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Cpp_Scope_Operator_In_C_Source_356
 */ 
class MutatorFrontendAction_356 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(356)

private:
    class MutatorASTConsumer_356 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_356(Rewriter &R) : TheRewriter(R) {}
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

