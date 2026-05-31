//source file
#include "../include/array_initialization_in_nested_lambda_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LambdaExpr->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = lambdaSource.find("{");
      if (pos != std::string::npos) {
        std::string nestedArrayInit = "\nint nested[2] = { x[0][0], x[1][1] }; return nested[0];";
        lambdaSource.insert(pos + 1, nestedArrayInit);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasDescendant(varDecl(hasType(arrayType())))).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}