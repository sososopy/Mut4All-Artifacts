//header file
#pragma once
#include "Mutator_base.h"
#include <string>
#include <regex>

/**
 * Modify_Template_Usage_71
 */ 
class MutatorFrontendAction_71 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(71)

private:
    class MutatorASTConsumer_71 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_71(Rewriter &R) : TheRewriter(R) {}
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

