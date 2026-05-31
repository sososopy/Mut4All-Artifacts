//source file
#include "../include/Mutator_Replace_Loop_Condition_421.h"

// ========================================================================================================
#define MUT421_OUTPUT 1

void MutatorFrontendAction_421::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ForStmt = Result.Nodes.getNodeAs<clang::ForStmt>("ForLoop")) {
      //Filter nodes in header files
      if (!ForStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ForStmt->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = ForStmt->getCond();
      if (!condition)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string replacement;
      int choice = getrandom::getRandomIndex(1);
      if (choice) {
        replacement = "true";
      } else {
        replacement = "false";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(condition->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_421::MutatorASTConsumer_421::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = forStmt(hasCondition(integerLiteral())).bind("ForLoop");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}