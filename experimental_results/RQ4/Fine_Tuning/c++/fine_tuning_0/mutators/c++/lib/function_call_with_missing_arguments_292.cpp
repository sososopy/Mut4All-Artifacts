//source file
#include "../include/Function_Call_With_Missing_Arguments_292.h"

// ========================================================================================================
#define MUT292_OUTPUT 1

void MutatorFrontendAction_292::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto call = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto lparen = call.find("(");
      auto rparen = call.rfind(")");
      if (lparen != string::npos && rparen != string::npos) {
        call = call.substr(0, lparen + 1) + call.substr(rparen);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), call);
    }
}
  
void MutatorFrontendAction_292::MutatorASTConsumer_292::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(hasArgument(0, anything())).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}