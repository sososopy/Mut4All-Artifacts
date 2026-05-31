//source file
#include "../include/mutate_function_call_argument_292.h"

// ========================================================================================================
#define MUT292_OUTPUT 1

void MutatorFrontendAction_292::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      //Get the source code text of target node
      unsigned numArgs = CE->getNumArgs();
      if (numArgs > 0) {
        auto arg = CE->getArg(0);
        auto argText = Lexer::getSourceText(CharSourceRange::getTokenRange(arg->getSourceRange()), 
                                            *Result.SourceManager, 
                                            Result.Context->getLangOpts());
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedArg = argText.str() + " / 0"; // Introducing division by zero
        Rewrite.ReplaceText(arg->getSourceRange(), mutatedArg);
      }
    }
}
  
void MutatorFrontendAction_292::MutatorASTConsumer_292::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl())).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}