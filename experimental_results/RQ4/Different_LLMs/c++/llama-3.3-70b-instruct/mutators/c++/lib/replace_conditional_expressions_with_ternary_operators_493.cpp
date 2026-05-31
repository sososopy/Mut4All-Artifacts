//source file
#include "../include/Mutator_Replace_Conditional_Expressions_With_Ternary_Operators_493.h"

// ========================================================================================================
#define MUT493_OUTPUT 1

void MutatorFrontendAction_493::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IfStmt = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!IfStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IfStmt->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = IfStmt->getCond();
      auto thenStmt = IfStmt->getThen();
      auto elseStmt = IfStmt->getElse();
      if (!condition || !thenStmt)
        return;
      std::string conditionStr = stringutils::rangetoStr(*(Result.SourceManager), condition->getSourceRange());
      std::string thenStmtStr = stringutils::rangetoStr(*(Result.SourceManager), thenStmt->getSourceRange());
      std::string elseStmtStr = "";
      if (elseStmt) {
        elseStmtStr = stringutils::rangetoStr(*(Result.SourceManager), elseStmt->getSourceRange());
      }
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCode = conditionStr + " ? " + thenStmtStr + " : " + elseStmtStr;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(IfStmt->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_493::MutatorASTConsumer_493::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt().bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}