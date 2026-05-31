//source file
#include "../include/Constant_Expression_with_Undeclared_Identifier_in_Switch_Statement_102.h"

// ========================================================================================================
#define MUT102_OUTPUT 1

void MutatorFrontendAction_102::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::SwitchStmt>("Switch")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getCond()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      condition = "/*mut102*/undeclared_identifier";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getCond()->getSourceRange(), condition);
    }
}
  
void MutatorFrontendAction_102::MutatorASTConsumer_102::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = switchStmt().bind("Switch");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}