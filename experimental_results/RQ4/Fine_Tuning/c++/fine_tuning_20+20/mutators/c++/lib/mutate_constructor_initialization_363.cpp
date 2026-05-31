//source file
#include "../include/mutate_constructor_initialization_363.h"

// ========================================================================================================
#define MUT363_OUTPUT 1

void MutatorFrontendAction_363::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructors")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    llvm::outs() << content << '\n';
    auto init_list = DL->inits();
    bool flag = false;
    for (auto init : init_list) {
      if (init->isBaseInitializer()) {
        flag = true;
        auto base = init->getBaseClass();
        llvm::outs() << QualType(base, 0).getAsString() << '\n';
        auto base_range = init->getSourceRange();
        auto base_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    base_range);
        llvm::outs() << base_content << '\n';
        content.replace(content.find(base_content), base_content.length(), "");
      }
    }
    if (!flag) {
      if (base_classes.empty())
        return;
      auto index = getrandom::getRandomIndex(base_classes.size() - 1);
      auto target = base_classes[index];
      auto base_name = target->getNameAsString();
      llvm::outs() << base_name << '\n';
      if (content.find(':') == string::npos) {
        content.insert(content.find('{'), ": " + base_name + "()");
      } else {
        content.insert(content.find('{'), ", " + base_name + "()");
      }
    }
    content = "/*mut363*/" + content;
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  } else if (auto *BL =
                 Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClasses")) {
    if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   BL->getLocation()))
      return;
    base_classes.push_back(BL);
  }
}
  
void MutatorFrontendAction_363::MutatorASTConsumer_363::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxConstructorDecl().bind("Constructors");
    auto base_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl())))
                            .bind("BaseClasses");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.matchAST(Context);
}