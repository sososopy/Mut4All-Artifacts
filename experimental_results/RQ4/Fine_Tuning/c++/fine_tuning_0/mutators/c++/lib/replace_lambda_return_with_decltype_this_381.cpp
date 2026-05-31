//source file
#include "../include/Replace_Lambda_Return_With_Decltype_This_381.h"

// ========================================================================================================
#define MUT381_OUTPUT 1

void MutatorFrontendAction_381::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("Returns")) {
      if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RS->getBeginLoc()))
        return;
      if (RS->getRetValue() == nullptr)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             RS->getSourceRange());
      auto lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas");
      if (lambda == nullptr)
        return;
      auto parent = lambda->getLambdaClass()->getParent();
      if (parent == nullptr)
        return;
      if (parent->isRecord())
        return;
      llvm::outs() << "/*mut381*/";
      content = "return decltype(this)();";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_381::MutatorASTConsumer_381::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = returnStmt().bind("Returns");
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.matchAST(Context);
}