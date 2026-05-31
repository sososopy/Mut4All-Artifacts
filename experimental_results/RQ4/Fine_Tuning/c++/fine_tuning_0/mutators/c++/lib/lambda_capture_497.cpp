//source file
#include "../include/Lambda_Capture_497.h"

// ========================================================================================================
#define MUT497_OUTPUT 1

void MutatorFrontendAction_497::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    cur_class = DL;
  } else if (auto *LB = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
    if (!LB || !Result.Context->getSourceManager().isWrittenInMainFile(
                   LB->getBeginLoc()))
      return;
    if (!LB->hasExplicitParameters())
      return;
    if (LB->captures().empty())
      return;
    auto captures = LB->captures();
    for (auto capture : captures) {
      if (capture.capturesThis()) {
        auto fields = cur_class->fields();
        if (fields.empty())
          return;
        auto field = *fields.begin();
        auto fieldname = field->getNameAsString();
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(capture.getLocation(),capture.getLocation()),
            fieldname);
        return;
      }
    }
  }
}
  
void MutatorFrontendAction_497::MutatorASTConsumer_497::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto lambda_matcher = lambdaExpr().bind("Lambdas");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(lambda_matcher, &callback);
  matchFinder.matchAST(Context);
}