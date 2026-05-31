//source file
#include "../include/lambda_with_nested_requires_687.h"

// ========================================================================================================
#define MUT687_OUTPUT 1

void MutatorFrontendAction_687::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  LambdaExpr->getSourceRange());

      std::string nestedLambda = R"(
        [] {
            return requires(T inner) { { inner } -> doesnt_matter; };
        }();
      )";

      size_t insertPos = lambdaSource.rfind("};");
      if (insertPos != std::string::npos) {
          lambdaSource.insert(insertPos, nestedLambda);
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_687::MutatorASTConsumer_687::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(lambdaExpr().bind("LambdaExpr")));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}