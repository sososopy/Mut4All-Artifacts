//source file
#include "../include/modify_array_initialization_with_casts_695.h"

// ========================================================================================================
#define MUT695_OUTPUT 1

void MutatorFrontendAction_695::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *initExpr = Result.Nodes.getNodeAs<clang::CStyleCastExpr>("castExpr")) {
      //Filter nodes in header files
      if (!initExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     initExpr->getExprLoc()))
        return;
      
      // Check if the cast type is (int *)
      if (initExpr->getTypeAsWritten()->isPointerType()) {
        auto pointeeType = initExpr->getTypeAsWritten()->getPointeeType();
        if (pointeeType->isIntegerType()) {
          //Perform mutation on the source code text by applying string replacement
          auto castRange = initExpr->getTypeInfoAsWritten()->getTypeLoc().getSourceRange();
          Rewrite.ReplaceText(castRange, "void *");
        }
      }
    }
}
  
void MutatorFrontendAction_695::MutatorASTConsumer_695::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cStyleCastExpr(hasDestinationType(pointerType(pointee(isInteger())))).bind("castExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}