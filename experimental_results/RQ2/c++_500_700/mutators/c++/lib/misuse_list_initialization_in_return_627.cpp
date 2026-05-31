//source file
#include "../include/misuse_list_initialization_in_return_627.h"

// ========================================================================================================
#define MUT627_OUTPUT 1

void MutatorFrontendAction_627::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RT = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnStmt")) {
      //Filter nodes in header files
      if (!RT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RT->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto returnRange = RT->getSourceRange();
      auto returnText = Lexer::getSourceText(CharSourceRange::getTokenRange(returnRange), 
                                             *Result.SourceManager, 
                                             Result.Context->getLangOpts());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedReturn = "return {}; // mutated by misuse_list_initialization_in_return_627";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(returnRange, mutatedReturn);
    }
}
  
void MutatorFrontendAction_627::MutatorASTConsumer_627::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = returnStmt().bind("ReturnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}