//source file
#include "../include/argument_type_mismatch_builtin_function_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      //Check if the function is a built-in function like __builtin_assume_aligned
      if (auto *FD = CE->getDirectCallee()) {
          if (FD->getBuiltinID() != 0) {
              //Get the source code text of target node
              auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
              
              //Perform mutation by replacing the first argument with an integer
              if (CE->getNumArgs() > 0) {
                  auto firstArg = CE->getArg(0);
                  auto argSourceText = stringutils::rangetoStr(*(Result.SourceManager), firstArg->getSourceRange());
                  
                  //Assume an integer is available in the scope. Replace the first argument with '42'.
                  std::string mutatedCall = sourceText;
                  auto pos = mutatedCall.find(argSourceText);
                  if (pos != std::string::npos) {
                      mutatedCall.replace(pos, argSourceText.length(), "42");
                  }
                  
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCall);
              }
          }
      }
    }
}
  
void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(functionDecl(hasAttr(clang::attr::Builtin)))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}