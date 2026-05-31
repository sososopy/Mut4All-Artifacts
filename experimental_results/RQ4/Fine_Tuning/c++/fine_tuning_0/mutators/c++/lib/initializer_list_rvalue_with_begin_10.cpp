//source file
#include "../include/Initializer_List_Rvalue_With_Begin_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *IL = Result.Nodes.getNodeAs<clang::InitListExpr>("InitLists")) {
      if (!IL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IL->getBeginLoc()))
        return;
      if (IL->isSemanticForm() == false)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             IL->getSourceRange());
      content = "/*mut10*/" + content + ".begin()";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(IL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = initListExpr().bind("InitLists");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}