//source file
#include "../include/modify_va_arg_with_incompatible_type_404.h"

// ========================================================================================================
#define MUT404_OUTPUT 1

void MutatorFrontendAction_404::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("vaArgCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto exprText = Lexer::getSourceText(CharSourceRange::getTokenRange(CE->getSourceRange()), 
                                           *Result.SourceManager, 
                                           Result.Context->getLangOpts());
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = exprText.str();
      size_t pos = mutatedText.find("va_arg");
      if (pos != std::string::npos) {
          size_t start = mutatedText.find('(', pos);
          size_t end = mutatedText.find(')', start);
          if (start != std::string::npos && end != std::string::npos) {
              std::string originalType = mutatedText.substr(start + 1, end - start - 1);
              std::string incompatibleType = "uint8_t(*)(void)";
              mutatedText.replace(start + 1, end - start - 1, incompatibleType);
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_404::MutatorASTConsumer_404::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(hasName("va_arg")))).bind("vaArgCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}