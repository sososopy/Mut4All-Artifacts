//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Semicolon_In_Record_165
 */ 
class MutatorFrontendAction_165 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(165)

private:
    class MutatorASTConsumer_165 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_165(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

