//source file
#include "../include/transform_lambda_to_nested_generic_lambda_402.h"

// ========================================================================================================
#define MUT402_OUTPUT 1

void MutatorFrontendAction_402::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;

      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());
      size_t captureEnd = lambdaSource.find("]") + 1;
      size_t bodyStart = lambdaSource.find("{", captureEnd);

      std::string outerTemplate = "template<typename T>";
      std::string innerLambda = "[y](auto z) { return x + y + z; };";

      std::string mutatedLambda = lambdaSource.substr(0, captureEnd) + "(auto y) " +
                                  outerTemplate + lambdaSource.substr(captureEnd, bodyStart - captureEnd) +
                                  innerLambda + lambdaSource.substr(bodyStart);

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), mutatedLambda);
    }
}
  
void MutatorFrontendAction_402::MutatorASTConsumer_402::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}