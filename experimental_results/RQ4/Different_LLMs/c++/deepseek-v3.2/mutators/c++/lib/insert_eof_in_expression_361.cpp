//source file
#include "../include/Insert_EOF_In_Expression_361.h"

// ========================================================================================================
#define MUT361_OUTPUT 1

void MutatorFrontendAction_361::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BE = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryExpr")) {
      //Filter nodes in header files
      if (!BE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BE->getBeginLoc()))
        return;
      //Get the source code text of target node
      SourceLocation opLoc = BE->getOperatorLoc();
      SourceManager &SM = *Result.SourceManager;
      //Perform mutation on the source code text by applying string replacement
      //Insert EOF after operator token
      Rewrite.InsertTextAfterToken(opLoc, "");
      //Remove all code after this point
      SourceLocation endLoc = BE->getEndLoc();
      CharSourceRange range = CharSourceRange::getCharRange(endLoc, SM.getLocForEndOfFile(SM.getMainFileID()));
      Rewrite.RemoveText(range);
    }
}
  
void MutatorFrontendAction_361::MutatorASTConsumer_361::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator().bind("BinaryExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}