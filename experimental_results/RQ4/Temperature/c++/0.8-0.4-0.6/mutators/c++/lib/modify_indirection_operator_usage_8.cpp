//source file
#include "../include/modify_indirection_operator_usage_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UE = Result.Nodes.getNodeAs<clang::UnaryOperator>("IndirectionOp")) {
      if (!UE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UE->getBeginLoc()))
        return;

      auto *IL = dyn_cast<IntegerLiteral>(UE->getSubExpr()->IgnoreParenImpCasts());
      if (!IL)
        return;

      std::string varName = "validVar";
      std::string ptrName = "ptr";
      std::string replacement = "int " + varName + " = 10;\nint* " + ptrName + " = &" + varName + ";\n";
      replacement += "*" + ptrName;

      SourceLocation startLoc = UE->getBeginLoc();
      SourceLocation endLoc = UE->getEndLoc();
      Rewrite.ReplaceText(SourceRange(startLoc, endLoc), replacement);
    }
}
  
void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = unaryOperator(hasOperatorName("*"), hasUnaryOperand(integerLiteral())).bind("IndirectionOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}