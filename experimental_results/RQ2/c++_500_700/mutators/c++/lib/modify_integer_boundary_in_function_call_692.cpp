//source file
#include "../include/modify_integer_boundary_in_function_call_692.h"
#include "llvm/ADT/SmallString.h"

// ========================================================================================================
#define MUT692_OUTPUT 1

void MutatorFrontendAction_692::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("funcCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;

      for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
        if (auto *IL = llvm::dyn_cast<clang::IntegerLiteral>(CE->getArg(i))) {
          // Check if the integer is a constant or can be evaluated at compile time
          if (IL->getType()->isIntegerType()) {
            llvm::APInt value = IL->getValue();
            // Modify the integer parameter to a boundary value
            llvm::APInt newValue = llvm::APInt::getSignedMinValue(64);
            llvm::SmallString<16> newValueStr;
            newValue.toString(newValueStr, 10, true, false);
            newValueStr += "LL";
            //Perform mutation on the source code text by applying string replacement
            Rewrite.ReplaceText(IL->getSourceRange(), newValueStr);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_692::MutatorASTConsumer_692::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(hasAnyArgument(integerLiteral())).bind("funcCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}