//source file
#include "../include/Swap_Base_Class_Order_In_Structs_488.h"

// ========================================================================================================
#define MUT488_OUTPUT 1

void MutatorFrontendAction_488::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto bases = DL->bases();
      if (std::distance(bases.begin(), bases.end()) < 2)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      std::vector<string> base_names;
      for (auto base : bases) {
        auto base_name = stringutils::rangetoStr(*(Result.SourceManager),
                                                 base.getSourceRange());
        base_names.push_back(base_name);
      }
      std::reverse(base_names.begin(), base_names.end());
      auto pos = content.find(':');
      content.erase(pos);
      content.insert(pos, ": ");
      for (auto base_name : base_names) {
        content.insert(pos + 2, base_name + ", ");
      }
      content.erase(content.rfind(','));
      llvm::outs() << content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_488::MutatorASTConsumer_488::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.matchAST(Context);
}