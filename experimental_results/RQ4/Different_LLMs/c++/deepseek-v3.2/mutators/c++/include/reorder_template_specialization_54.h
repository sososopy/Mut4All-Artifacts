//header file
#pragma once
#include "Mutator_base.h"

/**
 * Reorder_Template_Specialization_54
 */ 
class MutatorFrontendAction_54 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(54)

private:
    class MutatorASTConsumer_54 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_54(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateSpecializationDecl*> specializations;
        std::vector<const clang::ClassTemplateDecl*> primaryTemplates;
    };
};

