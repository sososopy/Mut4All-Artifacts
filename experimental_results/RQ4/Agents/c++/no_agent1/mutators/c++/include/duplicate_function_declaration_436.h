//header file
#pragma once
#include "Mutator_base.h"

/**
 * Duplicate_Function_Declaration_436
 */ 

class MutatorFrontendAction_436 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(436)
private:
    class MutatorASTConsumer_436 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_436(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> visitedFunctions;
    };
};

