//source file
#include "../include/replace_constexpr_callable_with_dynamic_621.h"

// ========================================================================================================
#define MUT621_OUTPUT 1

void MutatorFrontendAction_621::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callableExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto callableSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                    CE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCallable = "std::deque<int>{1,2,3}";
      std::string mutatedSource = "/*mut621*/" + mutatedCallable;

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedSource);
    }
}
  
void MutatorFrontendAction_621::MutatorASTConsumer_621::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(hasDescendant(lambdaExpr())).bind("callableExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}