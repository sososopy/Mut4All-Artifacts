//source file
#include "../include/Replace_Lambda_Capture_With_Default_Capture_286.h"

// ========================================================================================================
#define MUT286_OUTPUT 1

void MutatorFrontendAction_286::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getBeginLoc().getLocWithOffset(0)))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   LT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (lambdaText.find("[") != string::npos && lambdaText.find("]") != string::npos) {
        int start = lambdaText.find("[");
        int end = lambdaText.find("]");
        string captureList = lambdaText.substr(start + 1, end - start - 1);
        if (!captureList.empty()) {
          string defaultCapture = (getrandom::getRandomIndex(1) == 0) ? "[=]" : "[&]";
          lambdaText.replace(start, end - start + 1, defaultCapture);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), lambdaText);
    }
}
  
void MutatorFrontendAction_286::MutatorASTConsumer_286::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}