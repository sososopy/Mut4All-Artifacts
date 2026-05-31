//source file
#include "../include/swap_conditional_operators_139.h"

// ========================================================================================================
#define MUT139_OUTPUT 1

void MutatorFrontendAction_139::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto opText = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getSourceRange()), 
                                         *Result.SourceManager, 
                                         Result.Context->getLangOpts()).str();
      
      //Perform mutation on the source code text by applying string replacement
      if (BO->getOpcode() == BO_LT) {
        opText.replace(opText.find("<"), 1, ">");
      } else if (BO->getOpcode() == BO_GT) {
        opText.replace(opText.find(">"), 1, "<");
      } else if (BO->getOpcode() == BO_LE) {
        opText.replace(opText.find("<="), 2, ">=");
      } else if (BO->getOpcode() == BO_GE) {
        opText.replace(opText.find(">="), 2, "<=");
      } else {
        return;
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), opText);
    }
}
  
void MutatorFrontendAction_139::MutatorASTConsumer_139::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = binaryOperator(
        anyOf(hasOperatorName("<"), hasOperatorName(">"), 
              hasOperatorName("<="), hasOperatorName(">="))
    ).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}