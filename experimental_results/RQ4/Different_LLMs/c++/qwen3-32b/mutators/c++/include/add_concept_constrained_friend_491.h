//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Concept_Constrained_Friend_491
 */ 
class MutatorFrontendAction_491 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(491)

private:
    class MutatorASTConsumer_491 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_491(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::ConceptDecl *> availableConcepts;
    };
};

