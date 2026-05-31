//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Type_Qualifier_In_Template_Alias_315
 */ 
class MutatorFrontendAction_315 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(315)

private:
    class MutatorASTConsumer_315 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_315(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeAliasTemplateDecl *> cur_alias;
    };
};

