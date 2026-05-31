//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Concept_Requires_Call_To_Constrained_Function_68
 */ 
class MutatorFrontendAction_68 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(68)

private:
    class MutatorASTConsumer_68 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_68(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> constrainedFunctions;
        std::vector<const clang::ConceptDecl*> conceptDecls;
    };
};

