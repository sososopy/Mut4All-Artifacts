//source file
#include "../include/ReplaceLambdaWithFunctionPointer_174.h"

// ========================================================================================================
#define MUT174_OUTPUT 1

void MutatorFrontendAction_174::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!LE ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaText = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(LE->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string funcPtrText = std::string("int (*funcPtr)() = ") + lambdaText.str();
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(LE->getSourceRange()), funcPtrText);
    }
}
  
void MutatorFrontendAction_174::MutatorASTConsumer_174::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}