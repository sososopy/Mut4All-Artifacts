//source file
#include "../include/Modify_Lambda_Default_Arguments_426.h"

// ========================================================================================================
#define MUT426_OUTPUT 1

void MutatorFrontendAction_426::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            LE->getSourceRange());
      auto params = LE->getLambdaClass()->getLambdaCallOperator()->parameters();
      for (auto param : params) {
        if (param->hasDefaultArg() == false) {
          lambda.insert(lambda.rfind(')'), "=0");
        } else {
          auto defaultarg = param->getDefaultArg();
          auto defaultarg_str = stringutils::rangetoStr(
              *(Result.SourceManager), defaultarg->getSourceRange());
          llvm::outs() << defaultarg_str << '\n';
          if (defaultarg_str == "0") {
            lambda.replace(lambda.find('='), 2, "=1");
          } else
            lambda.replace(lambda.find('='), 2, "=0");
        }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambda);
    }
}
  
void MutatorFrontendAction_426::MutatorASTConsumer_426::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}