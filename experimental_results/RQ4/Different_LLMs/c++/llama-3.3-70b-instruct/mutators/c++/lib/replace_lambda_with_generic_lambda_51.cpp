//source file
#include "../include/Replace_Lambda_With_Generic_Lambda_51.h"

// ========================================================================================================
#define MUT51_OUTPUT 1

void MutatorFrontendAction_51::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                 L->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedLambdaText = lambdaText;
      size_t pos = mutatedLambdaText.find('[');
      if (pos != std::string::npos) {
        mutatedLambdaText.insert(pos, "auto ");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(L->getSourceRange()), mutatedLambdaText);
    }
}
  
void MutatorFrontendAction_51::MutatorASTConsumer_51::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}