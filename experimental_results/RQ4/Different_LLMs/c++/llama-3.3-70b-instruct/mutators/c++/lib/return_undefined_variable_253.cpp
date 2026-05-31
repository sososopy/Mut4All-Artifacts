//source file
#include "../include/Mutator_Return_Undefined_Variable_253.h"

// ========================================================================================================
#define MUT253_OUTPUT 1

void MutatorFrontendAction_253::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnStmt")) {
      //Filter nodes in header files
      if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RS->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                               RS->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      declaration = "return undefined_variable;";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_253::MutatorASTConsumer_253::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = returnStmt().bind("ReturnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}