//source file
#include "../include/create_empty_base_class_190.h"

// ========================================================================================================
#define MUT190_OUTPUT 1

void MutatorFrontendAction_190::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    for (std::size_t j = 0; j < base_classes.size(); ++j) {
      if (DL->isDerivedFrom(base_classes[j])) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               base_classes[j]->getSourceRange());
        if (content.rfind('}') != string::npos)
          content.insert(content.rfind('}'), "/*mut190*/");
        llvm::outs() << content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(base_classes[j]->getSourceRange()),
            content);
      }
    }
  } else if (auto *BL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Base")) {
    if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   BL->getLocation()))
      return;
    if ((!BL->isStruct() && !BL->isClass()) || BL->isLambda())
      return;
    base_classes.push_back(BL);
  }
}

void MutatorFrontendAction_190::MutatorASTConsumer_190::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto base_matcher = cxxRecordDecl().bind("Base");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.matchAST(Context);
}