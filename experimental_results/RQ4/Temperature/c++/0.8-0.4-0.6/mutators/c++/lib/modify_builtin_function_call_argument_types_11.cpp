//source file
#include "../include/modify_builtin_function_call_argument_types_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinCall")) {
      //Filter nodes in header files
      if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CallExpr->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto numArgs = CallExpr->getNumArgs();
      if (numArgs > 0) {
          //Perform mutation on the source code text by applying string replacement
          auto argToReplace = CallExpr->getArg(0);
          std::string replacement = "3.14"; // Example replacement with a floating-point literal
          Rewrite.ReplaceText(argToReplace->getSourceRange(), replacement);
      }
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(functionDecl(hasName("__builtin_fpclassify")))).bind("BuiltinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}