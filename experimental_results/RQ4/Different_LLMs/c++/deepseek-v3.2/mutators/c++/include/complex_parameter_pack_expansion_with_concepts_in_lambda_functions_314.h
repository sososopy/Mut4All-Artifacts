//header file
#pragma once
#include "Mutator_base.h"

/**
 * Complex_Parameter_Pack_Expansion_With_Concepts_In_Lambda_Functions_314
 */ 
class MutatorFrontendAction_314 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(314)

private:
    class MutatorASTConsumer_314 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_314(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> targetFunctions;
    };
};

