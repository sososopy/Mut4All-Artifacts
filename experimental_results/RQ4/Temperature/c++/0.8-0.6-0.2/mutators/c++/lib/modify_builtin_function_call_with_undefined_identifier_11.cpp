//source file
#include "../include/modify_builtin_function_call_with_undefined_identifier_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinCall")) {
      //Filter nodes in header files
      if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getExprLoc()))
        return;

      //Get the source code text of target node
      auto callText = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(FC->getSourceRange()),
          *Result.SourceManager, clang::LangOptions());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCall = callText.str();
      size_t pos = mutatedCall.find_last_of(")");
      if (pos != std::string::npos) {
          mutatedCall.insert(pos, ", undefinedVar");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(FC->getSourceRange(), mutatedCall);
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(hasName("__builtin_fpclassify")))).bind("BuiltinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}