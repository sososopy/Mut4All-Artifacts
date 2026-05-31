//source file
#include "../include/replace_operator_with_function_call_277.h"

// ========================================================================================================
#define MUT277_OUTPUT 1

void MutatorFrontendAction_277::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lhs = stringutils::rangetoStr(*(Result.SourceManager), BO->getLHS()->getSourceRange());
      auto rhs = stringutils::rangetoStr(*(Result.SourceManager), BO->getRHS()->getSourceRange());
      auto op = BO->getOpcodeStr().str();

      //Perform mutation on the source code text by applying string replacement
      std::string funcCall = "perform_" + op + "(" + lhs + ", " + rhs + ")";
      funcCall = "/*mut277*/" + funcCall;

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), funcCall);
    }
}
  
void MutatorFrontendAction_277::MutatorASTConsumer_277::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator().bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}