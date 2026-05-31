//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Template_Specialization_Type_Mismatch_In_Variadic_Template_Parameter_Pack_124
 */ 
class MutatorFrontendAction_124 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(124)

private:
    class MutatorASTConsumer_124 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_124(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        std::vector<const clang::CallExpr*> targetCalls;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        
    };
};

