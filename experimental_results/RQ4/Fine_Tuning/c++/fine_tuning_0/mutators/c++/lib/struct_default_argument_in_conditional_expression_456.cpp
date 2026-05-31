//source file
#include "../include/Struct_Default_Argument_in_Conditional_Expression_456.h"

// ========================================================================================================
#define MUT456_OUTPUT 1

void MutatorFrontendAction_456::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isCompleteDefinition())
        return;
      if (ST->isLambda())
        return;
      auto ctors = ST->ctors();
      for (auto ctor : ctors) {
        if (ctor->getNumParams() == 0)
          continue;
        if (ctor->getNumParams() == 1) {
          if (ctor->getParamDecl(0)->hasDefaultArg()) {
            target_struct = ST;
            break;
          }
        } else {
          for (unsigned i = 0; i < ctor->getNumParams(); ++i) {
            if (ctor->getParamDecl(i)->hasDefaultArg()) {
              target_struct = ST;
              break;
            }
          }
        }
      }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::ConditionalOperator>(
                   "ConditionalExpr")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      if (target_struct == nullptr)
        return;
      auto true_expr = CE->getTrueExpr();
      auto false_expr = CE->getFalseExpr();
      auto choice = getrandom::getRandomIndex(1);
      auto struct_name = target_struct->getNameAsString();
      auto replace_expr = "/*mut456*/" + struct_name + "{";
      if (choice)
        replace_expr +=
            stringutils::rangetoStr(*(Result.SourceManager), true_expr->getSourceRange()) +
            "}";
      else
        replace_expr +=
            stringutils::rangetoStr(*(Result.SourceManager), false_expr->getSourceRange()) +
            "}";
      if (choice)
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(true_expr->getSourceRange()),
            replace_expr);
      else
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(false_expr->getSourceRange()),
            replace_expr);
    }
}
  
void MutatorFrontendAction_456::MutatorASTConsumer_456::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    auto conditional_expr_matcher =
        conditionalOperator().bind("ConditionalExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(conditional_expr_matcher, &callback);
    matchFinder.matchAST(Context);
}