//source file
#include "../include/replace_arithmetic_operators_with_functions_117.h"

// ========================================================================================================
#define MUT117_OUTPUT 1

void MutatorFrontendAction_117::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;

      //Get the source code text of target node
      auto lhs = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getLHS()->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
      auto rhs = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getRHS()->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());

      std::string functionCall;
      switch (BO->getOpcode()) {
        case BO_Add:
          functionCall = "add(" + lhs.str() + ", " + rhs.str() + ")";
          break;
        case BO_Sub:
          functionCall = "subtract(" + lhs.str() + ", " + rhs.str() + ")";
          break;
        case BO_Mul:
          functionCall = "multiply(" + lhs.str() + ", " + rhs.str() + ")";
          break;
        case BO_Div:
          functionCall = "divide(" + lhs.str() + ", " + rhs.str() + ")";
          break;
        default:
          return; // Only replace specific operators
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), functionCall);
    }
}
  
void MutatorFrontendAction_117::MutatorASTConsumer_117::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(hasOperatorName("+")).bind("BinaryOp");
    matcher = binaryOperator(hasOperatorName("-")).bind("BinaryOp");
    matcher = binaryOperator(hasOperatorName("*")).bind("BinaryOp");
    matcher = binaryOperator(hasOperatorName("/")).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}