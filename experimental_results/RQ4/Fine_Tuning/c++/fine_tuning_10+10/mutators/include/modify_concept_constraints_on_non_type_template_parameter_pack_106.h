//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Concept_Constraints_On_Non_Type_Template_Parameter_Pack_106
 */ 
class MutatorFrontendAction_106 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(106)

private:
    class MutatorASTConsumer_106 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_106(Rewriter &R) : TheRewriter(R) {}
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

