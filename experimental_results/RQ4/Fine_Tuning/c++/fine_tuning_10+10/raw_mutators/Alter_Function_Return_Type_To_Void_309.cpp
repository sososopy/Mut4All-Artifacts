//header file
#pragma once
#include "Mutator_base.h"

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

// ========================================================================================================
#define MUT309_OUTPUT 1

void MutatorFrontendAction_309::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
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
        if (auto *Body = FD->getBody()) {
          for (auto &Stmt : Body->children()) {
            if (auto *ReturnStmt = dyn_cast<ReturnStmt>(Stmt)) {
              Rewrite.RemoveText(ReturnStmt->getSourceRange());
            }
          }
        }
      }
    }

    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CallExpr->getBeginLoc()))
        return;

      const FunctionDecl *Callee = CallExpr->getDirectCallee();
      if (!Callee || originalReturnTypes.find(Callee) == originalReturnTypes.end())
        return;

      // Remove the assignment if the function call was used in an assignment
      if (auto *BinOp = dyn_cast<BinaryOperator>(CallExpr->getParent())) {
        if (BinOp->isAssignmentOp()) {
          Rewrite.RemoveText(BinOp->getSourceRange());
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