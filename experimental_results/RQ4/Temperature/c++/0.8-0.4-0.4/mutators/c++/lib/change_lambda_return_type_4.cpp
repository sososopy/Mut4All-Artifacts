//source file
#include "../include/Change_Lambda_Return_Type_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      // Check if the lambda has an explicit return type
      if (LambdaExpr->hasExplicitResultType()) {
        // Get the source code text of target node
        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    LambdaExpr->getSourceRange());
        // Perform mutation on the source code text by applying string replacement
        size_t arrowPos = lambdaSource.find("->");
        if (arrowPos != std::string::npos) {
          size_t bracePos = lambdaSource.find('{', arrowPos);
          if (bracePos != std::string::npos) {
            lambdaSource.replace(arrowPos, bracePos - arrowPos, "-> int (int)");
            lambdaSource.insert(bracePos + 1, " int y; std::cout << y; ");
          }
        }
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
      }
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}