//source file
#include "../include/Insert_Nested_Block_In_Lambda_495.h"

// ========================================================================================================
#define MUT495_OUTPUT 1

void MutatorFrontendAction_495::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            LE->getSourceRange());
      auto captures = LE->capture_begin();
      if (captures == LE->capture_end())
        return;
      auto capture = *captures;
      auto capture_name = capture.getCapturedVar()->getNameAsString();
      auto body = LE->getBody();
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      body_str = "{\n^{\n" + body_str + "\n}(" + capture_name + ");\n}\n";
      llvm::outs() << body_str << '\n';
      lambda = "/*mut495*/" + lambda;
      Rewrite.ReplaceText(LE->getBeginLoc(), 0, lambda);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          body_str);
    }
}
  
void MutatorFrontendAction_495::MutatorASTConsumer_495::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}