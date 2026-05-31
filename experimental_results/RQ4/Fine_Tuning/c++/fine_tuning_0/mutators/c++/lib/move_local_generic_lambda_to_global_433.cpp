//source file
#include "../include/Move_Local_Generic_Lambda_To_Global_433.h"

// ========================================================================================================
#define MUT433_OUTPUT 1

void MutatorFrontendAction_433::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    if (DL->isGenericLambda() == false)
      return;
    llvm::outs() << "generic lambda found\n";
    cur_lambdas.push_back(DL);
  } else if (auto *FL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Funcs")) {
    if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FL->getBeginLoc()))
      return;
    if (!FL->hasBody())
      return;
    for (auto lambda : cur_lambdas) {
      if (FL->getSourceRange().fullyContains(lambda->getSourceRange())) {
        auto lambda_content = stringutils::rangetoStr(
            *(Result.SourceManager), lambda->getSourceRange());
        lambda_content =
            "/*mut433*/" + lambda_content + ";\n";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                lambda->getSourceRange()),
                            lambda->getLambdaClass()->getNameAsString());
        Rewrite.ReplaceText(FL->getBeginLoc(), 0, lambda_content);
      }
    }
  }
}

void MutatorFrontendAction_433::MutatorASTConsumer_433::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    auto func_matcher = functionDecl().bind("Funcs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}