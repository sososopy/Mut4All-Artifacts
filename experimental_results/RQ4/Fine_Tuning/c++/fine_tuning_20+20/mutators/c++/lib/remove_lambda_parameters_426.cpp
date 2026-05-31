//source file
#include "../include/mutator_426.h"

// ========================================================================================================
#define MUT426_OUTPUT 1

void MutatorFrontendAction_426::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto lp = lambda.find('(');
      auto rp = lambda.find(')');
      if (lp == string::npos || rp == string::npos)
        return;
      lambda = lambda.replace(lp + 1, rp - lp - 1, "");
      lambda = "/*mut426*/" + lambda;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambda);
    }
}
  
void MutatorFrontendAction_426::MutatorASTConsumer_426::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}