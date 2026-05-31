//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Binary_Operator_In_Constexpr_Conditional_220
 */ 
class MutatorFrontendAction_220 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(220)

private:
    class MutatorASTConsumer_220 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_220(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_binary_operator_in_constexpr_conditional_220.h"

// ========================================================================================================
#define MUT220_OUTPUT 1

void MutatorFrontendAction_220::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;

      //Get the source code text of target node
      auto lhsExpr = BO->getLHS();
      auto rhsExpr = BO->getRHS();
      
      //Perform mutation on the source code text by applying string replacement
      if (rhsExpr->getType()->isIntegralOrEnumerationType()) {
          std::string replacement = "&(" + clang::Lexer::getSourceText(
              clang::CharSourceRange::getTokenRange(rhsExpr->getSourceRange()),
              *Result.SourceManager, Result.Context->getLangOpts()).str() + ")";
          
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(rhsExpr->getSourceRange(), replacement);
      }
    }
}
  
void MutatorFrontendAction_220::MutatorASTConsumer_220::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(
        hasOperatorName("<"),
        hasLHS(expr().bind("lhs")),
        hasRHS(expr(hasType(isInteger())).bind("rhs")),
        hasAncestor(functionDecl(isConstexpr()))
    ).bind("BinaryOp");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}