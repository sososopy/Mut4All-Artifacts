//header file
#pragma once
#include "Mutator_base.h"

/**
 * VLA_VaArg_Cast_404
 */ 
class MutatorFrontendAction_404 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(404)

private:
    class MutatorASTConsumer_404 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_404(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT404_OUTPUT 1

void MutatorFrontendAction_404::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("CSCast")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto *SubExpr = MT->getSubExpr();
      if (!SubExpr)
        return;
      if (auto *CallExpr = dyn_cast<CallExpr>(SubExpr)) {
        auto *Callee = CallExpr->getCallee();
        if (!Callee)
          return;
        if (auto *DeclRefExpr = dyn_cast<DeclRefExpr>(Callee)) {
          auto *FuncDecl = dyn_cast<FunctionDecl>(DeclRefExpr->getDecl());
          if (!FuncDecl || FuncDecl->getNameAsString() != "va_arg")
            return;
        }
      }
      QualType DestType = MT->getTypeAsWritten();
      if (!DestType->isPointerType())