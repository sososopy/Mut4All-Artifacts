//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_Namespace_To_Invalid_Alias_223
 */ 
class MutatorFrontendAction_223 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(223)
private:
    class MutatorASTConsumer_223 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_223(Rewriter &R) : TheRewriter(R) {}
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

