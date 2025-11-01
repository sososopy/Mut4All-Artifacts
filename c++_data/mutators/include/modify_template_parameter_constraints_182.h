//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Parameter_Constraints_182
 */ 
class MutatorFrontendAction_182 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(182)

private:
    class MutatorASTConsumer_182 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_182(Rewriter &R) : TheRewriter(R) {}
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

