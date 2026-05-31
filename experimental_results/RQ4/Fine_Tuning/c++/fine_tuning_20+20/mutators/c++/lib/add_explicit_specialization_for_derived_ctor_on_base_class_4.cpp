//source file
#include "../include/add_explicit_specialization_for_derived_ctor_on_base_class_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      derived_classes.push_back(DL);
    } else if (auto *BL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Base")) {
      if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BL->getLocation()))
        return;
      if (BL->isCompleteDefinition() == false)
        return;
      llvm::outs() << BL->getNameAsString() << '\n';
      auto ctors = BL->ctors();
      for (auto ctor : ctors) {
        if (ctor->getDescribedFunctionTemplate() != nullptr) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 ctor->getSourceRange());
          for (auto derived : derived_classes) {
            content += "\n/*mut4*/template<>\n";
            content += BL->getNameAsString() + "::" +
                       ctor->getNameAsString() + "(";
            content += derived->getNameAsString() + "){}\n";
          }
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(ctor->getSourceRange()), content);
        }
      }
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto base_matcher = cxxRecordDecl().bind("Base");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.matchAST(Context);
}