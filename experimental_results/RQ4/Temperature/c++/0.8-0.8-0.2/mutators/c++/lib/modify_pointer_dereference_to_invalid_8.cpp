//source file
#include "../include/modify_pointer_dereference_to_invalid_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UnaryExpr = Result.Nodes.getNodeAs<clang::UnaryOperator>("ptrDeref")) {
      //Filter nodes in header files
      if (!UnaryExpr || !Result.SourceManager->isWrittenInMainFile(
                     UnaryExpr->getExprLoc()))
        return;
      
      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto DerefSourceRange = UnaryExpr->getSourceRange();
      auto DerefText = Lexer::getSourceText(
          CharSourceRange::getTokenRange(DerefSourceRange), SM, LangOptions());

      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText = "*(int*)0";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DerefSourceRange), MutatedText);
    }
}
  
void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = unaryOperator(hasOperatorName("*"),
                                 hasUnaryOperand(hasType(pointerType()))
                                ).bind("ptrDeref");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}