//source file
#include "../include/annotation_token_misuse_285.h"

// ========================================================================================================
#define MUT285_OUTPUT 1

void MutatorFrontendAction_285::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;
      
      //Get the source code text of target node
      auto lhs = stringutils::rangetoStr(*(Result.SourceManager), BO->getLHS()->getSourceRange());
      auto rhs = stringutils::rangetoStr(*(Result.SourceManager), BO->getRHS()->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "@" + lhs + " = " + rhs + ";";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_285::MutatorASTConsumer_285::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(isAssignmentOperator()).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}