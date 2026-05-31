//source file
#include "../include/insert_unreachable_code_block_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("FuncBody")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager),
                                              FS->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      size_t insertPos = bodyText.find("{") + 1;
      std::string unreachableCode = "\nif (false) { /*mut9*/ int unreachable = 0; }\n";
      bodyText.insert(insertPos, unreachableCode);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), bodyText);
    }
}
  
void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = compoundStmt(hasParent(functionDecl())).bind("FuncBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}