//source file
#include "../include/insert_unreachable_statement_500.h"

// ========================================================================================================
#define MUT500_OUTPUT 1

void MutatorFrontendAction_500::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::CompoundStmt>("FunctionBody")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto body = stringutils::rangetoStr(*(Result.SourceManager), FS->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string unreachableStmt = "\n/*mut500*/__builtin_unreachable();\n";
      body.insert(body.find_last_of('}'), unreachableStmt);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), body);
    }
}
  
void MutatorFrontendAction_500::MutatorASTConsumer_500::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = compoundStmt().bind("FunctionBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}