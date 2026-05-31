//source file
#include "../include/Modify_Array_Indexing_264.h"

// ========================================================================================================
#define MUT264_OUTPUT 1

void MutatorFrontendAction_264::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ArraySubscriptExpr>("ArraySubscriptExpr")) {
      //Filter nodes in header files
      if (!MT ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the index with a new index
      std::string newIndex = "newIndex";
      declaration.replace(declaration.find('[') + 1, declaration.find(']') - declaration.find('[') - 1, newIndex);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_264::MutatorASTConsumer_264::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = arraySubscriptExpr().bind("ArraySubscriptExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}