//source file
#include "../include/Remove_Attribute_173.h"

// ========================================================================================================
#define MUT173_OUTPUT 1

void MutatorFrontendAction_173::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AT = Result.Nodes.getNodeAs<clang::Attr>("Attributes")) {
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      auto attr = stringutils::rangetoStr(*(Result.SourceManager),
                                          AT->getRange());
      llvm::outs() << attr << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AT->getRange()),
                          "/*mut173*/");
    }
}
  
void MutatorFrontendAction_173::MutatorASTConsumer_173::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::attr().bind("Attributes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}