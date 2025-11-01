//source file
#include "../include/invalid_decltype_in_generic_lambda_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("GenericLambda")) {
      //Filter nodes in header files
      if (!LE || !Result.SourceManager->isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      auto pos = lambdaSource.find("return ");
      if (pos != std::string::npos) {
          lambdaSource.insert(pos, "-> decltype(undefined_variable) ");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("GenericLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}