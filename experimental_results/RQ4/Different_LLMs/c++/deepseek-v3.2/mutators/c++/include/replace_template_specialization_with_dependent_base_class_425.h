//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Specialization_With_Dependent_Base_Class_425
 */ 
class MutatorFrontendAction_425 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(425)

private:
    class MutatorASTConsumer_425 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_425(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> templateDecls;
    };
};

