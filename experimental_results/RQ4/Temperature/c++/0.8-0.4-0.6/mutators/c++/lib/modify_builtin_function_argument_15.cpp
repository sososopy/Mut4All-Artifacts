//source file
#include "../include/modify_builtin_function_argument_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      if (CE->getNumArgs() < 2)
        return;

      auto *firstArg = CE->getArg(0);
      auto *secondArg = CE->getArg(1);

      if (!firstArg->getType()->isPointerType())
        return;

      llvm::APSInt intValue;
      Expr::EvalResult evalResult;
      if (!secondArg->EvaluateAsInt(evalResult, *Result.Context) || !evalResult.Val.getInt().isPowerOf2())
        return;

      //Perform mutation on the source code text by applying string replacement
      std::string replacementText = "0"; // Replace with an integer literal
      Rewrite.ReplaceText(firstArg->getSourceRange(), replacementText);
    }
}
  
void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(functionDecl(hasName("__builtin_assume_aligned")))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}