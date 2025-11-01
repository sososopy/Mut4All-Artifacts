//header file
#pragma once
#include "Mutator_base.h"

/**
 * Annotation_Token_Misuse_285
 */ 
class MutatorFrontendAction_285 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(285)

private:
    class MutatorASTConsumer_285 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_285(Rewriter &R) : TheRewriter(R) {}
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

