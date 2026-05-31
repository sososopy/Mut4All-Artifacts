//source file
#include "../include/replace_control_flow_statements_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CS = Result.Nodes.getNodeAs<clang::CompoundStmt>("compoundStmt")) {
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getBeginLoc()))
        return;

      for (auto &stmt : CS->body()) {
        if (auto *BS = llvm::dyn_cast<clang::BreakStmt>(stmt)) {
          auto replacementText = "/*mut25*/int placeholder = 0;";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(BS->getSourceRange()), replacementText);
        }
      }
    }
}
  
void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = compoundStmt(hasDescendant(breakStmt())).bind("compoundStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}