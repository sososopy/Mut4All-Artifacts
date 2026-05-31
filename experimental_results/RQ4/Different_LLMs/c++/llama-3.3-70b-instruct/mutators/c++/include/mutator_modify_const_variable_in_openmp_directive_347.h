//header file
#pragma once
#include "Mutator_base.h"
#include <regex>

/**
 * Modify_Const_Variable_In_Openmp_Directive_347
 */ 
class MutatorFrontendAction_347 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(347)

private:
    class MutatorASTConsumer_347 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_347(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

