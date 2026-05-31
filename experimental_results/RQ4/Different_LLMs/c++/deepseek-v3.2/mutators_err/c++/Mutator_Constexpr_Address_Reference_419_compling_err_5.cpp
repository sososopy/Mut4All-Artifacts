//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Constexpr_Address_Reference_419
 */ 
class MutatorFrontendAction_419 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(419)

private:
    class MutatorASTConsumer_419 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_419(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> constexpr_vars;
        std::vector<const clang::CXXRecordDecl*> constexpr_structs;
    };
};

//source file
#include "../include/Mutator_Constexpr_Address_Reference_419.h"

// ========================================================================================================
#define MUT419_OUTPUT 1

void MutatorFrontendAction_419::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprVar")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if it's a constexpr variable declaration
      if (!MT->isConstexpr())
        return;
      //Get the initializer expression
      const Expr* init = MT->getInit();
      if (!init)
        return;
      //Check if initializer is not a simple literal (to avoid trivial cases)
      if (isa<clang::IntegerLiteral>(init) || isa<clang::FloatingLiteral>(init) || isa<clang::StringLiteral>(init