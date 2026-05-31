//source file
#include "../include/divide_by_zero_in_function_615.h"

// ========================================================================================================
#define MUT615_OUTPUT 1

void MutatorFrontendAction_615::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("divisionOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;

      // Ensure the operator is a division
      if (BO->getOpcode() == clang::BinaryOperatorKind::BO_Div) {
        // Get the divisor expression
        auto *divisor = BO->getRHS();

        // Check if the divisor is not already zero
        if (auto *intLiteral = llvm::dyn_cast<clang::IntegerLiteral>(divisor)) {
          if (intLiteral->getValue() == 0)
            return; // Already zero, no need to mutate
        }

        // Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(divisor->getSourceRange(), "0");
      }
    }
}
  
void MutatorFrontendAction_615::MutatorASTConsumer_615::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(hasOperatorName("/")).bind("divisionOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}