//source file
#include "../include/Modify_Switch_Control_Expression_102.h"

// ========================================================================================================
#define MUT102_OUTPUT 1

void MutatorFrontendAction_102::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::SwitchStmt>("switchStmt")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getSwitchLoc()))
        return;

      auto controlExpr = ST->getCond();
      if (!controlExpr)
        return;

      auto controlExprText = stringutils::rangetoStr(*(Result.SourceManager), controlExpr->getSourceRange());
      std::string mutatedExpr = "undeclaredVar"; // Mutation logic: Replace with undeclared identifier

      Rewrite.ReplaceText(controlExpr->getSourceRange(), mutatedExpr);
    }
}
  
void MutatorFrontendAction_102::MutatorASTConsumer_102::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = switchStmt().bind("switchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}