//source file
#include "../include/Mutator_ConditionalExpressionArrayMutation_498.h"

// ========================================================================================================
#define MUT498_OUTPUT 1

void MutatorFrontendAction_498::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::ConditionalOperator>(("ConditionalExpression"))) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto cond = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (CE->getLHS()->getType()->isArrayType() && CE->getRHS()->getType()->isArrayType()) {
        auto lhsType = cast<clang::ArrayType>(CE->getLHS()->getType())->getElementType();
        auto rhsType = cast<clang::ArrayType>(CE->getRHS()->getType())->getElementType();
        if (lhsType != rhsType) {
          // Replace the type of one of the arrays with a different type
          auto mutatedCond = cond;
          mutatedCond.replace(mutatedCond.find(lhsType.getAsString()), lhsType.getAsString().length(), rhsType.getAsString());
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCond);
        }
      } else if (CE->getLHS()->getType()->isArrayType()) {
        // If only one of the operands is an array, introduce a new array type
        auto arrayType = cast<clang::ArrayType>(CE->getLHS()->getType())->getElementType();
        auto mutatedCond = cond;
        mutatedCond.replace(mutatedCond.find(":"), 1, ": " + arrayType.getAsString() + " c[] = {0};");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCond);
      } else if (CE->getRHS()->getType()->isArrayType()) {
        // If only one of the operands is an array, introduce a new array type
        auto arrayType = cast<clang::ArrayType>(CE->getRHS()->getType())->getElementType();
        auto mutatedCond = cond;
        mutatedCond.replace(mutatedCond.find(":"), 1, ": " + arrayType.getAsString() + " c[] = {0};");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCond);
      }
    }
}

void MutatorFrontendAction_498::MutatorASTConsumer_498::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = conditionalOperator().bind("ConditionalExpression");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}