//source file
#include "../include/alter_block_statements_with_invalid_control_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BS = Result.Nodes.getNodeAs<clang::CompoundStmt>("blockStmt")) {
      if (!BS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BS->getLBracLoc()))
        return;

      auto &SM = Rewrite.getSourceMgr();
      auto &LangOpts = Result.Context->getLangOpts();
      auto StartLoc = BS->getLBracLoc().getLocWithOffset(1);
      auto EndLoc = BS->getRBracLoc().getLocWithOffset(-1);

      llvm::StringRef BlockContent = Lexer::getSourceText(
          CharSourceRange::getTokenRange(StartLoc, EndLoc), SM, LangOpts);

      std::string MutatedContent = BlockContent.str() + "\nif (0) break;";

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), MutatedContent);
    }
}
  
void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = compoundStmt(hasParent(ifStmt())).bind("blockStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}