//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Ambiguous_Member_193
 */ 
class MutatorFrontendAction_193 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(193)

private:
    class MutatorASTConsumer_193 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_193(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> processedClasses;
    };
};

