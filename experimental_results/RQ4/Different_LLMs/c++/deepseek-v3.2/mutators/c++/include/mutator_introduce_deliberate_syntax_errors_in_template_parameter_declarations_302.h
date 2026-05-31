//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Introduce_Deliberate_Syntax_Errors_In_Template_Parameter_Declarations_302
 */ 
class MutatorFrontendAction_302 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(302)

private:
    class MutatorASTConsumer_302 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_302(Rewriter &R) : TheRewriter(R) {}
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

