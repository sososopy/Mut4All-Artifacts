//header file
#pragma once
#include "Mutator_base.h"
#include <set>
#include <string>

/**
 * partial_specialization_unused_template_parameters_563
 */ 
class MutatorFrontendAction_563 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(563)

private:
    class MutatorASTConsumer_563 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_563(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> specializedClasses;
    };
};

