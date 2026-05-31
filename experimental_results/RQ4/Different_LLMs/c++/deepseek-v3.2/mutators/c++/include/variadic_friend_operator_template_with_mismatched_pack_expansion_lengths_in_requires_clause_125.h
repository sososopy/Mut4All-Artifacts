//header file
#pragma once
#include "Mutator_base.h"

/**
 * Variadic_Friend_Operator_Template_With_Mismatched_Pack_Expansion_Lengths_In_Requires_Clause_125
 */ 
class MutatorFrontendAction_125 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(125)

private:
    class MutatorASTConsumer_125 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_125(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> visitedTemplates;
    };
};

