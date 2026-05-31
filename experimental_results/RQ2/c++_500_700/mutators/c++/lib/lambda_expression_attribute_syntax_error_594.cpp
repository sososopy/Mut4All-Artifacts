//source file
#include "../include/lambda_expression_attribute_syntax_error_594.h"

// ========================================================================================================
#define MUT594_OUTPUT 1

void MutatorFrontendAction_594::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!Lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Lambda->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), Lambda->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t attrPos = lambdaSource.find("__attribute__((attr))");
      if (attrPos != std::string::npos) {
          lambdaSource.replace(attrPos, 20, "__attribute__((unfinished_attr({{");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Lambda->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_594::MutatorASTConsumer_594::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}