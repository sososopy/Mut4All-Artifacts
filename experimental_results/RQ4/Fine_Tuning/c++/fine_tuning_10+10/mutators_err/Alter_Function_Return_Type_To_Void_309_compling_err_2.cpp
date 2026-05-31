//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/Expr.h"
#include "clang/Frontend/FrontendAction.h"
#include <map>

/**
 * Alter_Function_Return_Type_To_Void_309
 */ 
class MutatorFrontendAction_309 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(309)

private:
    class MutatorASTConsumer_309 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_309(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const FunctionDecl*, std::string> originalReturnTypes;
    };
};

//source file
#include "../include/Alter_Function_Return_Type_To_Void_309.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT309_OUTPUT 1

void MutatorFrontendAction_309::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getReturnType()->isVoidType())
        return;

      if (FD->hasBody()) {
        originalReturnTypes[FD] = FD->getReturnType().getAsString();
        
        // Change the return type to void
        Rewrite.ReplaceText(FD->getReturnTypeSourceRange(), "void");

        // Remove return statements
        if (const auto *Body = FD->getBody()) {
          for (const auto &Stmt : Body->children()) {
            if (const auto *ReturnStmt = dyn_cast<ReturnStmt>(Stmt)) {
              Rewrite.RemoveText(ReturnStmt->getSourceRange());
            }
          }
        }
      }
    }

    if (const auto *CallExpr = Result.Nodes.getNodeAs<CallExpr>("FunctionCall")) {
      if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CallExpr->getBeginLoc()))
        return;

      const FunctionDecl *Callee = CallExpr->getDirectCallee();
      if (!Callee || originalReturnTypes.find(Callee) == originalReturnTypes.end())
        return;

      // Remove the assignment if the function call was used in an assignment
      if (const auto *Parent = Result.Nodes.getNodeAs<BinaryOperator>("Parent")) {
        if (Parent->isAssignmentOp()) {
          Rewrite.RemoveText(Parent->getSourceRange());
        }
      }
    }
}
  
void MutatorFrontendAction_309::MutatorASTConsumer_309::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl(unless(returns(voidType()))).bind("Function");
    StatementMatcher callMatcher = callExpr(callee(functionDecl())).bind("FunctionCall");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}