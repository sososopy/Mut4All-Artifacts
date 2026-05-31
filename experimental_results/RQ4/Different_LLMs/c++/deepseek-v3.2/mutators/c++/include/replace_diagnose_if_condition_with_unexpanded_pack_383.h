//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Diagnose_If_Condition_With_Unexpanded_Pack_383
 */ 
class MutatorFrontendAction_383 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(383)

private:
    class MutatorASTConsumer_383 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_383(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> templateFunctions;
    };
};

