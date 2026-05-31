//source file
#include "../include/Move_Lambda_Capture_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LC = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LC->getBeginLoc()))
        return;
      if (LC->capture_size() == 0)
        return;
      auto LBD = LC->getLambdaClass();
      auto captures = LBD->captures();
      auto capture_begin = captures.begin();
      auto capture_end = captures.end();
      string capture_str = "[";
      for (auto it = capture_begin; it != capture_end; ++it) {
        capture_str +=
            it->getCapturedVar()->getNameAsString() + "=std::move(" +
            it->getCapturedVar()->getNameAsString() + ")";
        if (it != capture_end - 1)
          capture_str += ",";
      }
      capture_str += "]";
      auto capture_range = LC->getIntroducerRange();
      llvm::outs() << capture_str << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(capture_range), capture_str);
    }
}
  
void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}