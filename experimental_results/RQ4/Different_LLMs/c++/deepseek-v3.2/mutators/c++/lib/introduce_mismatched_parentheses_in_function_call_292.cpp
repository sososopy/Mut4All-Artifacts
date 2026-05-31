//source file
#include "../include/Introduce_Mismatched_Parentheses_In_Function_Call_292.h"

// ========================================================================================================
#define MUT292_OUTPUT 1

void MutatorFrontendAction_292::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      SourceRange range = MT->getSourceRange();
      SourceLocation endLoc = range.getEnd();
      //Perform mutation on the source code text by applying string replacement
      //Find the closing parenthesis token
      Token token;
      if (!Lexer::getRawToken(endLoc, token, *Result.SourceManager, Result.Context->getLangOpts(), true)) {
        if (token.getKind() == tok::r_paren) {
          //Check if function call has arguments
          if (MT->getNumArgs() == 0) {
            //Insert '(' before ')'
            Rewrite.InsertTextBefore(endLoc, "(");
          } else {
            //Replace ')' with '('
            Rewrite.ReplaceText(endLoc, 1, "(");
          }
        }
      }
    }
}
  
void MutatorFrontendAction_292::MutatorASTConsumer_292::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(hasParent(compoundStmt())).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}