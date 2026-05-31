//header file
#pragma once
#include "Mutator_base.h"
#include <sstream>
/**
 * Mutate_Intrinsic_Invocation_296
 */ 
class MutatorFrontendAction_296 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(296)

private:
    class MutatorASTConsumer_296 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_296(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

