//source file
#include "../include/Replace_Member_Pointer_Operand_With_Null_258.h"

// ========================================================================================================
#define MUT258_OUTPUT 1

void MutatorFrontendAction_258::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BE = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BE->getBeginLoc()))
        return;
      
      // Check if operator is a comparison operator
      if (!BE->isComparisonOp()) return;
      
      // Get left and right operands
      Expr *LHS = BE->getLHS();
      Expr *RHS = BE->getRHS();
      
      // Check if at least one operand is a pointer to member
      bool LHSIsMemberPointer = LHS->getType()->isMemberPointerType();
      bool RHSIsMemberPointer = RHS->getType()->isMemberPointerType();
      
      if (!LHSIsMemberPointer && !RHSIsMemberPointer) return;
      
      // Determine which operand to replace
      bool replaceLHS = false;
      bool replaceRHS = false;
      
      if (LHSIsMemberPointer && RHSIsMemberPointer) {
          // Both are member pointers, randomly choose one
          int choice = getrandom::getRandomIndex(1);
          if (choice == 0) replaceLHS = true;
          else replaceRHS = true;
      } else if (LHSIsMemberPointer) {
          replaceLHS = true;
      } else {
          replaceRHS = true;
      }
      
      //Get the source code text of target node
      SourceRange targetRange;
      std::string replacementText = "0";
      
      if (replaceLHS) {
          targetRange = LHS->getSourceRange();
      } else {
          targetRange = RHS->getSourceRange();
      }
      
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(targetRange, replacementText);
    }
}
  
void MutatorFrontendAction_258::MutatorASTConsumer_258::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(
        anyOf(
            hasOperatorName("=="),
            hasOperatorName("!=="),
            hasOperatorName("<"),
            hasOperatorName(">"),
            hasOperatorName("<="),
            hasOperatorName(">=")
        ),
        anyOf(
            hasLHS(hasType(memberPointerType())),
            hasRHS(hasType(memberPointerType()))
        )
    ).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}