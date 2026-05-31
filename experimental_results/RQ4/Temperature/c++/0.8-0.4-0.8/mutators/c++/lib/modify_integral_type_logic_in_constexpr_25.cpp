//source file
#include "../include/modify_integral_type_logic_in_constexpr_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *IF = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
      if (!IF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IF->getIfLoc()))
        return;

      if (auto *Cond = dyn_cast<clang::BinaryOperator>(IF->getCond())) {
        if (Cond->getLHS()->getType()->isIntegralOrEnumerationType()) {
          auto LHS = stringutils::rangetoStr(*(Result.SourceManager), Cond->getLHS()->getSourceRange());
          std::string MutatedLHS = "static_cast<float>(" + LHS + ")";
          Rewrite.ReplaceText(Cond->getLHS()->getSourceRange(), MutatedLHS);
        }
      }
    }
}
  
void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = ifStmt(hasCondition(binaryOperator(hasLHS(expr(hasType(isInteger())))))).bind("ifStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}