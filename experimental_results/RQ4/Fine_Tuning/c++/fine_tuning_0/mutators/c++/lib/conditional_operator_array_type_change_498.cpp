//source file
#include "../include/Conditional_Operator_Array_Type_Change_498.h"

// ========================================================================================================
#define MUT498_OUTPUT 1

void MutatorFrontendAction_498::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CO = Result.Nodes.getNodeAs<clang::ConditionalOperator>("CondOp")) {
      if (!CO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CO->getBeginLoc()))
        return;
      auto TrueExpr = CO->getTrueExpr();
      auto FalseExpr = CO->getFalseExpr();
      auto TrueExprStr = stringutils::rangetoStr(*(Result.SourceManager),
                                                 TrueExpr->getSourceRange());
      auto FalseExprStr = stringutils::rangetoStr(*(Result.SourceManager),
                                                  FalseExpr->getSourceRange());
      llvm::outs() << "TrueExpr: " << TrueExprStr << "\n";
      llvm::outs() << "FalseExpr: " << FalseExprStr << "\n";
      if (TrueExpr->getType() != FalseExpr->getType()) {
        auto TrueExprType = TrueExpr->getType().getAsString();
        auto FalseExprType = FalseExpr->getType().getAsString();
        llvm::outs() << "TrueExprType: " << TrueExprType << "\n";
        llvm::outs() << "FalseExprType: " << FalseExprType << "\n";
        if (TrueExprType.find("[") != string::npos &&
            TrueExprType.find("]") != string::npos) {
          auto TrueExprTypeNew = TrueExprType;
          TrueExprTypeNew.replace(TrueExprTypeNew.find("["),
                                  TrueExprTypeNew.find("]") -
                                      TrueExprTypeNew.find("[") + 1,
                                  "[]");
          llvm::outs() << "TrueExprTypeNew: " << TrueExprTypeNew << "\n";
          auto TrueExprStrNew = TrueExprStr;
          TrueExprStrNew.replace(0, TrueExprType.size(), TrueExprTypeNew);
          llvm::outs() << "TrueExprStrNew: " << TrueExprStrNew << "\n";
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(TrueExpr->getSourceRange()),
              TrueExprStrNew);
        } else if (FalseExprType.find("[") != string::npos &&
                   FalseExprType.find("]") != string::npos) {
          auto FalseExprTypeNew = FalseExprType;
          FalseExprTypeNew.replace(FalseExprTypeNew.find("["),
                                   FalseExprTypeNew.find("]") -
                                       FalseExprTypeNew.find("[") + 1,
                                   "[]");
          llvm::outs() << "FalseExprTypeNew: " << FalseExprTypeNew << "\n";
          auto FalseExprStrNew = FalseExprStr;
          FalseExprStrNew.replace(0, FalseExprType.size(), FalseExprTypeNew);
          llvm::outs() << "FalseExprStrNew: " << FalseExprStrNew << "\n";
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(FalseExpr->getSourceRange()),
              FalseExprStrNew);
        }
      }
    }
}
  
void MutatorFrontendAction_498::MutatorASTConsumer_498::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conditionalOperator(
                       hasTrueExpression(hasType(arrayType())),
                       hasFalseExpression(hasType(arrayType())))
                       .bind("CondOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}