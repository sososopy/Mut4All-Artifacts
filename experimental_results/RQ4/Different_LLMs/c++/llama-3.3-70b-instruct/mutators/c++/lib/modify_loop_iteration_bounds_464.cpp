//source file
#include "../include/Modify_Loop_Iteration_Bounds_464.h"

// ========================================================================================================
#define MUT464_OUTPUT 1

void MutatorFrontendAction_464::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ST = Result.Nodes.getNodeAs<clang::ForStmt>("ForLoop")) {
      //Filter nodes in header files
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getInit()->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto loopInit = ST->getInit();
      auto loopCond = ST->getCond();
      auto loopInc = ST->getInc();
      if (!loopInit || !loopCond || !loopInc)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string loopInitStr = stringutils::rangetoStr(*(Result.SourceManager), loopInit->getSourceRange());
      std::string loopCondStr = stringutils::rangetoStr(*(Result.SourceManager), loopCond->getSourceRange());
      std::string loopIncStr = stringutils::rangetoStr(*(Result.SourceManager), loopInc->getSourceRange());
      int choice = getrandom::getRandomIndex(2);
      std::string mutatedLoopCondStr;
      if (choice == 0) {
        mutatedLoopCondStr = "i < -511";
      } else {
        mutatedLoopCondStr = "i < 18446744073709551615";
      }
      std::string mutatedLoopStr = loopInitStr + " " + mutatedLoopCondStr + " " + loopIncStr;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()), mutatedLoopStr);
    }
}
  
void MutatorFrontendAction_464::MutatorASTConsumer_464::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = forStmt().bind("ForLoop");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}