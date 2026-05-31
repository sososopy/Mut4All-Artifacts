//source file
#include "../include/Alter_Whitespace_In_Expressions_19292.h"

// ========================================================================================================
#define MUT192_OUTPUT 1

void MutatorFrontendAction_19292::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("binaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;
      //Get the source code text of target node
      auto sourceRange = BO->getSourceRange();
      auto text = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange),
                                       *Result.SourceManager, Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      for (char c : text) {
          if (c == ' ') continue; // Remove all spaces
          mutatedText += c;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(sourceRange, mutatedText);
    }
}
  
void MutatorFrontendAction_19292::MutatorASTConsumer_19292::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = binaryOperator(hasOperatorName("+"), hasOperatorName("-"),
                                  hasOperatorName("*"), hasOperatorName("/")).bind("binaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}