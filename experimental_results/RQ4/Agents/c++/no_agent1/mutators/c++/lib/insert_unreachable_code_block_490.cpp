//source file
#include "../include/insert_unreachable_code_block_490.h"

// ========================================================================================================
#define MUT490_OUTPUT 1

void MutatorFrontendAction_490::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("FuncBody")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLBracLoc()))
        return;

      //Get the source code text of target node
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager),
                                              FS->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string unreachableCode = "\n/*mut490*/if (false) { while(true) {} }\n";
      bodyText.insert(bodyText.find("{") + 1, unreachableCode);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), bodyText);
    }
}
  
void MutatorFrontendAction_490::MutatorASTConsumer_490::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = compoundStmt(hasParent(functionDecl())).bind("FuncBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}