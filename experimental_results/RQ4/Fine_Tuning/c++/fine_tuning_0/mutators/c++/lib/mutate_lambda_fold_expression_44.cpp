//source file
#include "../include/Mutate_Lambda_Fold_Expression_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;

      auto captures = MT->captures();
      for (auto capture : captures) {
        if (capture.capturesVariable()) {
          auto vardecl = capture.getCapturedVar();
          if (vardecl->getNameAsString() == "a") {
            auto capture_range = capture.getLocation();
            auto capture_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                       capture_range);
            capture_str += " , b";
            llvm::outs() << capture_str << '\n';
            Rewrite.ReplaceText(capture_range, capture_str);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}