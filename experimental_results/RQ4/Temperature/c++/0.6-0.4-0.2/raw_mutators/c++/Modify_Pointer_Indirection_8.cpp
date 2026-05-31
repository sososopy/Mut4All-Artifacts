//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_pointer_indirection_8
 */ 
class MutatorFrontendAction_8 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(8)

private:
    class MutatorASTConsumer_8 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_8(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/modify_pointer_indirection_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UO = Result.Nodes.getNodeAs<clang::UnaryOperator>("unaryOp")) {
      //Filter nodes in header files
      if (!UO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UO->getBeginLoc()))
        return;

      // Check if the operand is a pointer type
      if (UO->getOpcode() == clang::UO_Deref) {
        const clang::Expr *subExpr = UO->getSubExpr();
        if (subExpr->getType()->isPointerType()) {
          //Perform mutation on the source code text by applying string replacement
          std::string replacement = "*" + std::to_string(42); // Replace with an integer literal
          Rewrite.ReplaceText(UO->getSourceRange(), replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = unaryOperator(hasOperatorName("*")).bind("unaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}