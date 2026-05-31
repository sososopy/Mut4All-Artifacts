//source file
#include "../include/modify_cxxparenlistinitexpr_89.h"

// ========================================================================================================
#define MUT89_OUTPUT 1

void MutatorFrontendAction_89::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ParenListExpr>("ParenListExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;

      //Get the source code text of target node
      auto exprRange = MT->getSourceRange();
      std::string exprText = stringutils::rangetoStr(*(Result.SourceManager), exprRange);

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedExpr = exprText;
      if (MT->getNumExprs() > 1) {
          mutatedExpr = exprText.substr(0, exprText.find_last_of(',')) + ")";
      } else if (MT->getNumExprs() == 2) {
          std::swap(mutatedExpr[exprText.find(',') + 1], mutatedExpr[exprText.find(',') - 1]);
      } else if (MT->getNumExprs() > 0) {
          mutatedExpr.insert(exprText.find('(') + 1, exprText.substr(exprText.find('(') + 1, exprText.find(',') - exprText.find('(') - 1) + ", ");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(exprRange), mutatedExpr);
    }
}
  
void MutatorFrontendAction_89::MutatorASTConsumer_89::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = parenListExpr().bind("ParenListExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}