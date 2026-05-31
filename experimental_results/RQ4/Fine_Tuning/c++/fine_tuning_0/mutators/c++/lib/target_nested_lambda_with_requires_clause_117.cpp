//source file
#include "../include/Target_Nested_Lambda_with_Requires_Clause_117.h"

// ========================================================================================================
#define MUT117_OUTPUT 1

void MutatorFrontendAction_117::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("requires") == string::npos)
        return;
      if (content.find("...") != string::npos)
        return;
      if (content.find("return") == string::npos)
        return;
      content.insert(content.find_last_of('(') + 1, "auto... params");
      content.insert(content.find("}") - 1, "params...");
      content = "/*mut117*/" + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
  }

void MutatorFrontendAction_117::MutatorASTConsumer_117::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(lambdaExpr())).bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}