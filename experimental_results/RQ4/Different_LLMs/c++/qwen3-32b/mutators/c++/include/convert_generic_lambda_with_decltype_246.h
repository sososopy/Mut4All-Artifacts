//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_Generic_Lambda_With_Decltype_246
 */ 
class MutatorFrontendAction_246 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(246)

private:
    class MutatorASTConsumer_246 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_246(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite), hasProcessedLambda(false) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        bool hasProcessedLambda;
    };
};

