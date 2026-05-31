//source file
#include "../include/Mutator_Change_Ternary_To_If_Else_Statement_399.h"

// ========================================================================================================
#define MUT399_OUTPUT 1

void MutatorFrontendAction_399::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Conditional = Result.Nodes.getNodeAs<clang::ConditionalOperator>("Conditional")) {
      //Filter nodes in header files
      if (!Conditional || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Conditional->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = stringutils::rangetoStr(*(Result.SourceManager), Conditional->getCond()->getSourceRange());
      auto trueExpr = stringutils::rangetoStr(*(Result.SourceManager), Conditional->getTrueExpr()->getSourceRange());
      auto falseExpr = stringutils::rangetoStr(*(Result.SourceManager), Conditional->getFalseExpr()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string ifElseStatement = "if (" + condition + ") {\n" + trueExpr + "\n} else {\n" + falseExpr + "\n}";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Conditional->getSourceRange()), ifElseStatement);
    }
}
  
void MutatorFrontendAction_399::MutatorASTConsumer_399::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = conditionalOperator().bind("Conditional");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}