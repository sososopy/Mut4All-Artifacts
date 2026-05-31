//source file
#include "../include/Modify_Function_Call_Invalid_Arguments_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      //Filter nodes in header files
      if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CallExpr->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto originalArg = CallExpr->getArg(0);
      if (!originalArg)
        return;

      //Perform mutation on the source code text by applying string replacement
      auto originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(originalArg->getSourceRange()), 
                                               *Result.SourceManager, 
                                               Result.Context->getLangOpts());

      std::string mutatedText = originalText.str() + " + undefined_variable";

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(originalArg->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(hasArgument(0, expr())).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}