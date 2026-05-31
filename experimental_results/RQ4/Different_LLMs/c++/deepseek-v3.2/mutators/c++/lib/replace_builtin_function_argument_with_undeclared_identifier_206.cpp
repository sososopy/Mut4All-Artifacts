//source file
#include "../include/Mutator_Replace_Builtin_Function_Argument_With_Undeclared_Identifier_206.h"

// ========================================================================================================
#define MUT206_OUTPUT 1

void MutatorFrontendAction_206::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Check if this is a builtin function call
      const clang::FunctionDecl *FD = CE->getDirectCallee();
      if (!FD || !FD->getIdentifier() || !FD->getIdentifier()->getName().startswith("__builtin_"))
        return;
      
      // Get number of arguments
      unsigned numArgs = CE->getNumArgs();
      if (numArgs == 0)
        return;
      
      // Select a random argument to replace
      unsigned argIndex = getrandom::getRandomIndex(numArgs - 1);
      const clang::Expr *argExpr = CE->getArg(argIndex);
      
      // Get the source range of the argument
      clang::SourceRange argRange = argExpr->getSourceRange();
      if (argRange.isInvalid())
        return;
      
      // Generate a unique undeclared identifier
      std::string undeclaredName = "mut206_undeclared_" + std::to_string(getrandom::getRandomIndex(1000000));
      
      // Get the full call expression text
      std::string callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      
      // Get the argument text to replace
      std::string argText = stringutils::rangetoStr(*(Result.SourceManager), argRange);
      
      // Find and replace the argument in the call text
      size_t pos = callText.find(argText);
      if (pos == std::string::npos)
        return;
      
      callText.replace(pos, argText.length(), undeclaredName);
      callText = "/*mut206*/" + callText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), callText);
    }
}
  
void MutatorFrontendAction_206::MutatorASTConsumer_206::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(matchesName("__builtin_")))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}