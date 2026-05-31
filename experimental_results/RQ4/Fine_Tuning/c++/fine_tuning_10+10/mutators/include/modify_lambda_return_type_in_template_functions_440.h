//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Return_Type_In_Template_Functions_440
 */ 
class MutatorFrontendAction_440 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(440)

private:
    class MutatorASTConsumer_440 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_440(Rewriter &R) : TheRewriter(R) {}
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

