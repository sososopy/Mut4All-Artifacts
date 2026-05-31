//source file
#include "../include/modify_template_function_parameter_504.h"

// ========================================================================================================
#define MUT504_OUTPUT 1

void MutatorFrontendAction_504::Callback::run(const MatchFinder::MatchResult &Result) {
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
      size_t pos = lambdaSource.find('(');
      if (pos != std::string::npos) {
          lambdaSource.insert(pos + 1, "class _U, ");
          pos = lambdaSource.find(')', pos);
          if (pos != std::string::npos) {
              lambdaSource.insert(pos, ", _U u");
          }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_504::MutatorASTConsumer_504::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}