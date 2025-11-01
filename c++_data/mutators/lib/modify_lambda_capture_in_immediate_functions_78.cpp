//source file
#include "../include/modify_lambda_capture_in_immediate_functions_78.h"

// ========================================================================================================
#define MUT78_OUTPUT 1

void MutatorFrontendAction_78::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      std::set<std::string> capturedVars;

      for (auto it = LambdaExpr->capture_begin(); it != LambdaExpr->capture_end(); ++it) {
          if (it->capturesVariable()) {
              capturedVars.insert(it->getCapturedVar()->getNameAsString());
          }
      }

      for (const auto &var : localVars) {
          if (capturedVars.find(var) == capturedVars.end()) {
              std::string newCapture = "[" + var + "]";
              Rewrite.InsertText(LambdaExpr->getBeginLoc(), newCapture, true, true);
              break;
          }
      }
    }
}
  
void MutatorFrontendAction_78::MutatorASTConsumer_78::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr(hasAncestor(callExpr())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}