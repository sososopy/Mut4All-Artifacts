//source file
#include "../include/misname_function_identifiers_in_constexpr_conditions_305.h"

// ========================================================================================================
#define MUT305_OUTPUT 1

void MutatorFrontendAction_305::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ICE = Result.Nodes.getNodeAs<clang::IfStmt>("IfConstexpr")) {
      if (!ICE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ICE->getBeginLoc()))
        return;

      if (ICE->isConstexpr()) {
        auto condition = ICE->getCond();
        if (auto *callExpr = dyn_cast<CallExpr>(condition)) {
          if (auto *callee = callExpr->getDirectCallee()) {
            std::string originalName = callee->getNameInfo().getName().getAsString();
            if (originalName.empty()) return;

            std::string mutatedName = originalName;
            if (mutatedName.length() > 1) {
              std::swap(mutatedName[0], mutatedName[1]);
            }

            SourceLocation startLoc = callExpr->getBeginLoc();
            SourceLocation endLoc = Lexer::getLocForEndOfToken(callExpr->getEndLoc(), 0, *Result.SourceManager, Result.Context->getLangOpts());
            Rewrite.ReplaceText(SourceRange(startLoc, endLoc), mutatedName);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_305::MutatorASTConsumer_305::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = ifStmt(isConstexpr()).bind("IfConstexpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}