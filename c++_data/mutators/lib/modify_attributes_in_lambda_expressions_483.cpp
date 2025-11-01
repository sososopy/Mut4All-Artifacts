//source file
#include "../include/modify_attributes_in_lambda_expressions_483.h"

// ========================================================================================================
#define MUT483_OUTPUT 1

void MutatorFrontendAction_483::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      // Example mutation: Replace, Remove, or Add attributes
      if (lambdaSource.find("__attribute__") != std::string::npos) {
          // Replace existing attribute
          lambdaSource = std::regex_replace(lambdaSource, std::regex("__attribute__\\(\\(.*?\\)\\)"), "__attribute__((used))");
      } else {
          // Add a new attribute
          size_t pos = lambdaSource.find("[]");
          if (pos != std::string::npos) {
              lambdaSource.insert(pos + 2, " __attribute__((aligned(16)))");
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_483::MutatorASTConsumer_483::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}