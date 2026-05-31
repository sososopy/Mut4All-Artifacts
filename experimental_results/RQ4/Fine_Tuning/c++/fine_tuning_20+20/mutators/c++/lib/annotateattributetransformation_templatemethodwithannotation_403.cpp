//source file
#include "../include/AnnotateAttributeTransformation_TemplateMethodWithAnnotation_403.h"

// ========================================================================================================
#define MUT403_OUTPUT 1

void MutatorFrontendAction_403::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isCompleteDefinition())
      return;
    if (DL->isTemplated() == false)
      return;
    target_record = DL;
  } else if (auto *MT =
                 Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
    if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MT->getLocation()))
      return;
    if (MT->getParent() != target_record)
      return;
    if (MT->hasBody() == false)
      return;
    auto annotation = "[[clang::annotate(\"mut403\",9,(void)T{})]]";
    Rewrite.ReplaceText(MT->getBody()->getSourceRange(), annotation);
  }
}
  
void MutatorFrontendAction_403::MutatorASTConsumer_403::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto method_matcher = cxxMethodDecl().bind("Methods");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(method_matcher, &callback);
  matchFinder.matchAST(Context);
}