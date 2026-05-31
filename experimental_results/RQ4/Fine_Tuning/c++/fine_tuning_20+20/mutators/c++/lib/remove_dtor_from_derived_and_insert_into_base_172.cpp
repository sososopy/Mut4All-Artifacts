//source file
#include "../include/remove_dtor_from_derived_and_insert_into_base_172.h"

// ========================================================================================================
#define MUT172_OUTPUT 1

void MutatorFrontendAction_172::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto dtor = DL->getDestructor();
    if (dtor == nullptr)
      return;
    if (dtor->isImplicit() == true)
      return;
    auto dtor_range = dtor->getSourceRange();
    auto dtor_content = stringutils::rangetoStr(*(Result.SourceManager), dtor_range);
    dtor_content = "\n/*mut172*/" + dtor_content + '\n';
    for (auto base_class : base_classes) {
      if (DL->isDerivedFrom(base_class)) {
        llvm::outs() << base_class->getNameAsString() << '\n';
        Rewrite.ReplaceText(dtor_range, "");
        Rewrite.ReplaceText(base_class->getEndLoc(), 0, dtor_content);
      }
    }
  } else if (auto *BL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "Base")) { // If there is a return 0 statement in the function, replace the return value with the inserted static const
    if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   BL->getBeginLoc()))
      return;
    if (BL->isCompleteDefinition() == false)
      return;
    base_classes.push_back(BL);
  }
}

void MutatorFrontendAction_172::MutatorASTConsumer_172::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto base_matcher = cxxRecordDecl().bind("Base");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.matchAST(Context);
}