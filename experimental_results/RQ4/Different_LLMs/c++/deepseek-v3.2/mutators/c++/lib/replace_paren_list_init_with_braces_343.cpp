//source file
#include "../include/Replace_Paren_List_Init_With_Braces_343.h"

// ========================================================================================================
#define MUT343_OUTPUT 1

void MutatorFrontendAction_343::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("ConstructExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      SourceRange range = MT->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
      //Check if it's a parenthesized list initialization (not already braces)
      if (originalText.find('{') != std::string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      size_t openParen = originalText.find('(');
      if (openParen == std::string::npos)
        return;
      size_t closeParen = originalText.rfind(')');
      if (closeParen == std::string::npos)
        return;
      std::string mutatedText = originalText;
      mutatedText.replace(openParen, 1, "{");
      mutatedText.replace(closeParen, 1, "}");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), mutatedText);
    }
}
  
void MutatorFrontendAction_343::MutatorASTConsumer_343::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxConstructExpr(unless(isInTemplateInstantiation())).bind("ConstructExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}