//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_186
 */ 
class MutatorFrontendAction_186 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(186)

private:
    class MutatorASTConsumer_186 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_186(Rewriter &R) : TheRewriter(R) {}
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

