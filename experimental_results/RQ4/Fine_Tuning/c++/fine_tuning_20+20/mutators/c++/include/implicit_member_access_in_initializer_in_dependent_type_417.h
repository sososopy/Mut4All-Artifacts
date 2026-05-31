//header file
#pragma once
#include "Mutator_base.h"

/**
 * Implicit_Member_Access_In_Initializer_In_Dependent_Type_417
 */ 
class MutatorFrontendAction_417 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(417)

private:
    class MutatorASTConsumer_417 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_417(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::ClassTemplateDecl *> cur_templates;
    };
};

