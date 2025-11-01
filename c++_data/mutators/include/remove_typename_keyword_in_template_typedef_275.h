//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Typename_Keyword_In_Template_Typedef_275
 */ 
class MutatorFrontendAction_275 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(275)

private:
    class MutatorASTConsumer_275 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_275(Rewriter &R) : TheRewriter(R) {}
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

