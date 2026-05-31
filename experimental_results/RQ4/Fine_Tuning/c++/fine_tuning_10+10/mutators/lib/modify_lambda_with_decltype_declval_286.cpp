//source file
#include "../include/modify_lambda_with_decltype_declval_286.h"

// ========================================================================================================
#define MUT286_OUTPUT 1

void MutatorFrontendAction_286::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LambdaExpr->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto insertionPoint = lambdaSource.find_last_of('}');
      if (insertionPoint != std::string::npos) {
        std::string mutation = "\nusing ResultType = decltype(std::declval<decltype(l)>() + std::declval<int>());";
        lambdaSource.insert(insertionPoint, mutation);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_286::MutatorASTConsumer_286::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr(hasDescendant(declRefExpr(to(varDecl(hasType(decltypeType())))))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}