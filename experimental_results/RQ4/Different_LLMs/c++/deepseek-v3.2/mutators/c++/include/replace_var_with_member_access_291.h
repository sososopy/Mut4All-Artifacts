//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Var_With_Member_Access_291
 */ 
class MutatorFrontendAction_291 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(291)

private:
    class MutatorASTConsumer_291 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_291(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::RecordDecl*> structDecls;
        std::vector<const clang::VarDecl*> structVars;
        std::vector<const clang::DeclRefExpr*> candidateIdExprs;
    };
};

