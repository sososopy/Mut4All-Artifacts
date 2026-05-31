//source file
#include "../include/Lambda_Capture_Mutation_402.h"

// ========================================================================================================
#define MUT402_OUTPUT 1

void MutatorFrontendAction_402::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!Lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Lambda->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto capture = stringutils::rangetoStr(*(Result.SourceManager),
                                           Lambda->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      int choice = getrandom::getRandomIndex(3);
      if (choice == 0) {
        // Add a new capture to the lambda expression
        capture.insert(capture.find('[') + 1, "x, ");
      } else if (choice == 1) {
        // Remove an existing capture from the lambda expression
        size_t pos = capture.find(',');
        if (pos != string::npos) {
          capture.erase(pos, 1);
        }
      } else if (choice == 2) {
        // Modify an existing capture to capture a different variable
        size_t pos = capture.find('x');
        if (pos != string::npos) {
          capture.replace(pos, 1, "y");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Lambda->getSourceRange()), capture);
    }
}
  
void MutatorFrontendAction_402::MutatorASTConsumer_402::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}