//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Conflicting_Template_Specialization_74
 */ 
class MutatorFrontendAction_74 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(74)

private:
    class MutatorASTConsumer_74 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_74(Rewriter &R) : TheRewriter(R) {}
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
        const ClassTemplateDecl *target = nullptr;
        std::string target_name;
        std::string target_concept;
        std::string target_type;
    };
};

