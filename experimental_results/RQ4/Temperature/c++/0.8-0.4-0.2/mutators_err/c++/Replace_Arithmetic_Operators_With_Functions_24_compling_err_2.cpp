//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Arithmetic_Operators_With_Functions_24
 */ 
class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)

private:
    class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto lhs = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getLHS()->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
      auto rhs = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getRHS()->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());

      std::string replacement;
      switch (BO->getOpcode()) {
        case BO_Add:
          replacement = "add(" + lhs.str() + ", " + rhs.str() + ")";
          break;
        case BO_Sub:
          replacement = "subtract(" + lhs.str() + ", " + rhs.str() + ")";
          break;
        case BO_Mul:
          replacement = "multiply(" + lhs.str() + ", " + rhs.str() + ")";
          break;
        case BO_Div:
          replacement = "divide(" + lhs.str() + ", " + rhs.str() + ")";
          break;
        default:
          return;
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(hasOperatorName("+") || hasOperatorName("-") || hasOperatorName("*") || hasOperatorName("/")).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}