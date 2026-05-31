//source file
#include "../include/Remove_Switch_Statement_254.h"

// ========================================================================================================
#define MUT254_OUTPUT 1

void MutatorFrontendAction_254::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::SwitchStmt>("Switch")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      auto switch_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    MT->getSourceRange());
      llvm::outs() << switch_content;
      auto case_pos = switch_content.find("case");
      if (case_pos == string::npos)
        return;
      switch_content.erase(0, case_pos);
      llvm::outs() << switch_content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), switch_content);
    }
}
  
void MutatorFrontendAction_254::MutatorASTConsumer_254::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = switchStmt().bind("Switch");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}