//source file
#include "../include/Replace_Lambda_With_Templated_Fold_Expression_132.h"

// ========================================================================================================
#define MUT132_OUTPUT 1

void MutatorFrontendAction_132::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                LE->getSourceRange());
      llvm::outs() << "Original lambda: " << lambdaText << "\n";
      auto newLambdaText =
          "[]<typename... Args>(Args... args) { ((void)args, ...); }";
      lambdaText = "/*mut132*/" + std::string(newLambdaText);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()),
                          lambdaText);
    }
}
  
void MutatorFrontendAction_132::MutatorASTConsumer_132::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}