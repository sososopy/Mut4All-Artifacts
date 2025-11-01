//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_And_Type_Mismatch_In_Struct_241
 */ 
class MutatorFrontendAction_241 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(241)

private:
    class MutatorASTConsumer_241 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_241(Rewriter &R) : TheRewriter(R) {}
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

