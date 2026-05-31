//source file
#include "../include/Invalid_Range_Based_For_Loop_Condition_100.h"

// ========================================================================================================
#define MUT100_OUTPUT 1

void MutatorFrontendAction_100::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("ForRange")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      auto range = MT->getRangeInit();
      auto range_str = stringutils::rangetoStr(*(Result.SourceManager),
                                               range->getSourceRange());
      auto range_mut = range_str + " ? 1 : 2";
      llvm::outs() << range_mut << '\n';
      Rewrite.ReplaceText(range->getSourceRange(), range_mut);
    }
}
  
void MutatorFrontendAction_100::MutatorASTConsumer_100::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = cxxForRangeStmt().bind("ForRange");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}