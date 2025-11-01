//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Preprocessor_Directive_227
 */ 
class MutatorFrontendAction_227 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(227)

private:
    class MutatorASTConsumer_227 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_227(Rewriter &R) : TheRewriter(R) {}
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

