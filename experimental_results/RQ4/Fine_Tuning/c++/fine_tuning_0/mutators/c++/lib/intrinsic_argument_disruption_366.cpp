//source file
#include "../include/Intrinsic_Argument_Disruption_366.h"

// ========================================================================================================
#define MUT366_OUTPUT 1

void MutatorFrontendAction_366::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("Intrinsics")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      auto callee = MT->getCallee();
      auto callee_name = stringutils::rangetoStr(*(Result.SourceManager),
                                                 callee->getSourceRange());
      llvm::outs() << callee_name << '\n';
      if (callee_name[0] != '_' || callee_name[1] != '_')
        return;
      auto args = MT->getArgs();
      if (MT->getNumArgs() == 0)
        return;
      auto arg0 = stringutils::rangetoStr(*(Result.SourceManager),
                                          args[0]->getSourceRange());
      llvm::outs() << arg0 << '\n';
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      int choice = getrandom::getRandomIndex(1);
      if (choice) {
        content = callee_name + "(" + arg0 + ")";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
      }
    }
}
  
void MutatorFrontendAction_366::MutatorASTConsumer_366::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr().bind("Intrinsics");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}