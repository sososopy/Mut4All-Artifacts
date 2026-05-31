//header file
#pragma once
#include "Mutator_base.h"

/**
 * Use_Const_Var_As_Array_Initializer_299
 */ 
class MutatorFrontendAction_299 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(299)
    
    private:
      class MutatorASTConsumer_299 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_299(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::VarDecl *> const_vars;
          std::vector<const clang::VarDecl *> array_vars;
      };
    };

//source file
#include "../include/use_const_var_as_array_initializer_299.h"

// ========================================================================================================
#define MUT299_OUTPUT 1

void MutatorFrontendAction_299::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ConstVars")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isConstexpr() == false)
      return;
    const_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ArrayVars")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == false)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("OrdinaryVars")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType()->isArrayType() == true || DL->isConstexpr() == true)
      return;
    array_vars.push_back(DL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("InsertVar")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->