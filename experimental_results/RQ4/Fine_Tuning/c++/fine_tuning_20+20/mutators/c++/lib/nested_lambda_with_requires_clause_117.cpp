//source file
#include "../include/nested_lambda_with_requires_clause_117.h"

// ========================================================================================================
#define MUT117_OUTPUT 1

void MutatorFrontendAction_117::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      nested_lambdas.push_back(LE);
    } else if (auto *ILE = Result.Nodes.getNodeAs<clang::LambdaExpr>(
                   "InnermostLambda")) {
      if (!ILE || !Result.Context->getSourceManager().isWrittenInMainFile(
                      ILE->getBeginLoc()))
        return;
      if (ILE->hasExplicitParameters() == false)
        return;
      if (ILE->hasExplicitResultType() == false)
        return;
      auto params = ILE->getLambdaClass()->getLambdaCallOperator()->parameters();
      if (params.empty())
        return;
      auto first_param = params[0];
      if (first_param->getType().getAsString() != "auto")
        return;
      if (params.size() == 1)
        return;
      if (params.size() == 2) {
        if (params[1]->getType().getAsString() != "auto")
          return;
      }
      auto lambda_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    ILE->getSourceRange());
      if (params.size() == 2) {
        lambda_content.insert(lambda_content.find('{'),
                              " requires requires { obj.size(params...); }");
      } else {
        lambda_content.insert(lambda_content.find('{'),
                              " requires requires { obj.size(); }");
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(ILE->getSourceRange()), lambda_content);
    }
}
  
void MutatorFrontendAction_117::MutatorASTConsumer_117::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambda");
    auto innermost_lambda_matcher =
        lambdaExpr().bind("InnermostLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(innermost_lambda_matcher, &callback);
    matchFinder.matchAST(Context);
}