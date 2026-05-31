//source file
#include "../include/modify_lambda_return_type_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.SourceManager->isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto lambdaRange = LE->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);

      //Perform mutation on the source code text by applying string replacement
      size_t arrowPos = lambdaText.find("->");
      if (arrowPos != std::string::npos) {
          size_t bracePos = lambdaText.find("{", arrowPos);
          if (bracePos != std::string::npos) {
              lambdaText.insert(arrowPos + 2, "int(int)");
          }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}