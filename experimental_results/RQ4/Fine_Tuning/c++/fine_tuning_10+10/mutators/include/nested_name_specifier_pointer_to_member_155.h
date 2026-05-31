//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Name_Specifier_Pointer_To_Member_155
 */ 
class MutatorFrontendAction_155 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(155)

private:
    class MutatorASTConsumer_155 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_155(Rewriter &R) : TheRewriter(R) {}
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

