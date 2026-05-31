//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_213
 */ 
class MutatorFrontendAction_213 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(213)

private:
    class MutatorASTConsumer_213 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_213(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> baseClasses;
    };
};

