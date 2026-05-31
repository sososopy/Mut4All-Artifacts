//source file
#include "../include/modify_incomplete_static_assert_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      // Get the source code text of target node
      auto conditionRange = MT->getAssertExpr()->getSourceRange();
      auto messageRange = MT->getMessage()->getSourceRange();
      auto messageText = stringutils::rangetoStr(*(Result.SourceManager), messageRange);

      // Perform mutation on the source code text by removing the closing quotation mark and the parentheses
      if (messageText.back() == '"') {
          messageText.pop_back();
      }

      // Construct the mutated statement
      std::string mutatedText = "static_assert(" + stringutils::rangetoStr(*(Result.SourceManager), conditionRange) + ", " + messageText;

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}