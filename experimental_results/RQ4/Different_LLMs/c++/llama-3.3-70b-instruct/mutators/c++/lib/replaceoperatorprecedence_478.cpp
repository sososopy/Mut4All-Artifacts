//source file
#include "../include/ReplaceOperatorPrecedence_478.h"

// ========================================================================================================
#define REPLACEOPERATORPRECEDENCE_478_OUTPUT 1

void MutatorFrontendAction_478::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BE = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOperator")) {
      //Filter nodes in header files
      if (!BE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   BE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Add parentheses around the left operand
      size_t pos = declaration.find_first_of("+-*/%<>=!&|^");
      if (pos != string::npos) {
        string left = declaration.substr(0, pos);
        string right = declaration.substr(pos);
        declaration = "(" + left + ")" + right;
      }
      // Add parentheses around the right operand
      pos = declaration.find_last_of("+-*/%<>=!&|^");
      if (pos != string::npos) {
        string left = declaration.substr(0, pos + 1);
        string right = declaration.substr(pos + 1);
        declaration = left + "(" + right + ")";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BE->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_478::MutatorASTConsumer_478::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator().bind("BinaryOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}