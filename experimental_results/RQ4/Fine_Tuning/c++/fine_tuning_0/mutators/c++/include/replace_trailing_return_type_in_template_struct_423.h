//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Trailing_Return_Type_In_Template_Struct_423
 */ 
class MutatorFrontendAction_423 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(423)

private:
    class MutatorASTConsumer_423 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_423(Rewriter &R) : TheRewriter(R) {}
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

