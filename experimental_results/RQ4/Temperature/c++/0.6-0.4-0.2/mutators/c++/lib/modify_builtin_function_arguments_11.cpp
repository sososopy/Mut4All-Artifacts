//source file
#include "../include/modify_builtin_function_arguments_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
      //Filter nodes in header files
      if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CallExpr->getExprLoc()))
        return;
      
      const FunctionDecl *FD = CallExpr->getDirectCallee();
      if (!FD || !FD->getBuiltinID())
        return;

      //Get the source code text of target node
      std::string originalCall = Lexer::getSourceText(CharSourceRange::getTokenRange(CallExpr->getSourceRange()), 
                                                      *Result.SourceManager, 
                                                      Result.Context->getLangOpts()).str();
      
      //Perform mutation on the source code text by applying string replacement
      if (FD->getName().startswith("__builtin_fpclassify") && CallExpr->getNumArgs() > 1) {
        const Expr *arg = CallExpr->getArg(1);
        if (auto *intLiteral = dyn_cast<IntegerLiteral>(arg)) {
          std::string mutatedCall = originalCall;
          mutatedCall.replace(intLiteral->getBeginLoc().getRawEncoding(), 
                              intLiteral->getEndLoc().getRawEncoding() - intLiteral->getBeginLoc().getRawEncoding(), 
                              "1.5");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CallExpr->getSourceRange(), mutatedCall);
        }
      }
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(hasAttr(attr::Builtin)))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}