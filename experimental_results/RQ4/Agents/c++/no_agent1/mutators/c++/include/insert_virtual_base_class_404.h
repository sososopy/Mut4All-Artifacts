//header file
#pragma once
#include "Mutator_base.h"
#include <set>

/**
 * Insert_Virtual_Base_Class_404
 */ 
class MutatorFrontendAction_404 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(404)

private:
    class MutatorASTConsumer_404 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_404(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> visitedClasses;
    };
};

