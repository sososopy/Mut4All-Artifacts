//source file
#include "../include/malformed_attributes_368.h"

// ========================================================================================================
#define MUT368_OUTPUT 1

void MutatorFrontendAction_368::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AL = Result.Nodes.getNodeAs<clang::Attr>("Attr")) {
      //Filter nodes in header files
      if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AL->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             AL->getRange());
      //Perform mutation on the source code text by applying string replacement
      content = "";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AL->getRange()), content);
    }
}
  
void MutatorFrontendAction_368::MutatorASTConsumer_368::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::attr().bind("Attr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}