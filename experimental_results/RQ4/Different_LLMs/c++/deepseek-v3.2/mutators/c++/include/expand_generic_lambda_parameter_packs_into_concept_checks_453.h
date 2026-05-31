//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Generic_Lambda_Parameter_Packs_Into_Concept_Checks_453
 */ 
class MutatorFrontendAction_453 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(453)

private:
    class MutatorASTConsumer_453 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_453(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<std::string> availableConceptNames;
    };
};

