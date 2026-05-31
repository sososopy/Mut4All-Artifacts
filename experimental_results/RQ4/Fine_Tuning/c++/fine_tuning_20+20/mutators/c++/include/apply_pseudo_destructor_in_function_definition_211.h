//header file
#pragma once
#include "Mutator_base.h"

/**
 * Apply_Pseudo_Destructor_In_Function_Definition_211
 */ 
class MutatorFrontendAction_211 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(211)

private:
    class MutatorASTConsumer_211 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_211(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> cur_vars;
        std::vector<const clang::Expr *> cur_exprs;
    };
};

