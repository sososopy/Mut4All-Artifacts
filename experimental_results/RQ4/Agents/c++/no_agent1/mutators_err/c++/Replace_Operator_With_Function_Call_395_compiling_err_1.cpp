//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Operator_With_Function_Call_395
 */ 

class MutatorFrontendAction_395 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(395)
private:
    class MutatorASTConsumer_395 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_395(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_operator_with_function_call_395.h"

// ========================================================================================================
#define MUT395_OUTPUT 1

void MutatorFrontendAction_395::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;

      //Get the source code text of target node
      auto lhs = stringutils::rangetoStr(*(Result.SourceManager), BO->getLHS()->getSourceRange());
      auto rhs = stringutils::rangetoStr(*(Result.SourceManager), BO->getRHS()->getSourceRange());
      std::string opFunc;

      //Perform mutation on the source code text by applying string replacement
      switch (BO->getOpcode()) {
        case BO_Add:
          opFunc = "add";
          break;
        case BO_Sub:
          opFunc = "subtract";
          break;
        case BO_Mul:
          opFunc = "multiply";
          break;
        case BO_Div:
          opFunc = "divide";
          break;
        default:
          return; // Only mutate basic arithmetic operators
      }

      std::string replacement = opFunc + "(" + lhs + ", " + rhs + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_395::MutatorASTConsumer_395::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(hasOperatorName("+").bind("BinaryOp"),
                                              hasOperatorName("-").bind("BinaryOp"),
                                              hasOperatorName("*").bind("BinaryOp"),
                                              hasOperatorName("/").bind("BinaryOp"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}