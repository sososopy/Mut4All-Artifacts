//source file
#include "../include/modify_default_arguments_in_lambda_579.h"

// ========================================================================================================
#define MUT579_OUTPUT 1

void MutatorFrontendAction_579::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      for (auto *Param : LambdaExpr->getCallOperator()->parameters()) {
        if (Param->hasDefaultArg()) {
          auto DefaultArg = Param->getDefaultArg();
          if (DefaultArg) {
            auto DefaultArgText = Lexer::getSourceText(CharSourceRange::getTokenRange(DefaultArg->getSourceRange()), 
                                                       *Result.SourceManager, Result.Context->getLangOpts());
            std::string MutatedArg = "({ return 42; " + std::string(DefaultArgText) + "; })";
            Rewrite.ReplaceText(DefaultArg->getSourceRange(), MutatedArg);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_579::MutatorASTConsumer_579::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasAnyCapture(anything())).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}