//source file
#include "../include/Modify_Usage_of_CXXParenListInitExpr_111.h"

// ========================================================================================================
#define MUT111_OUTPUT 1

void MutatorFrontendAction_111::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ParenListExpr>("CXXParenListInitExpr")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLParenLoc()))
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      llvm::outs() << text << "\n";
      text = "/*mut111*/{" + text.substr(1, text.length() - 2) + "}";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), text);
    }
}
  
void MutatorFrontendAction_111::MutatorASTConsumer_111::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = parenListExpr().bind("CXXParenListInitExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}