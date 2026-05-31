//source file
#include "../include/invalid_decltype_in_lambda_246.h"

// ========================================================================================================
#define MUT246_OUTPUT 1

void MutatorFrontendAction_246::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;

      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("decltype") != string::npos)
        return;
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), "\n/*mut246*/decltype(y) y;");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_246::MutatorASTConsumer_246::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}