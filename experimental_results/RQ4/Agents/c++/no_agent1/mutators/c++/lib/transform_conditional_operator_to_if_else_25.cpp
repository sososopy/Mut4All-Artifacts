//source file
#include "../include/transform_conditional_operator_to_if_else_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CO = Result.Nodes.getNodeAs<clang::ConditionalOperator>("CondOp")) {
      //Filter nodes in header files
      if (!CO || !Result.SourceManager->isWrittenInMainFile(
                     CO->getBeginLoc()))
        return;
        
      //Get the source code text of target node
      auto condition = stringutils::rangetoStr(*(Result.SourceManager), CO->getCond()->getSourceRange());
      auto trueExpr = stringutils::rangetoStr(*(Result.SourceManager), CO->getTrueExpr()->getSourceRange());
      auto falseExpr = stringutils::rangetoStr(*(Result.SourceManager), CO->getFalseExpr()->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string ifElseStatement = "/*mut25*/if (" + condition + ") {\n" + trueExpr + ";\n} else {\n" + falseExpr + ";\n}";

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CO->getSourceRange()), ifElseStatement);
    }
}
  
void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = conditionalOperator().bind("CondOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}