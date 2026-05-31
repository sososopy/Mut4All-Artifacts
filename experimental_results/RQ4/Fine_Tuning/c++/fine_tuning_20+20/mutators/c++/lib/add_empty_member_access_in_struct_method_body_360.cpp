//source file
#include "../include/Add_Empty_Member_Access_In_Struct_Method_Body_360.h"

// ========================================================================================================
#define MUT360_OUTPUT 1

void MutatorFrontendAction_360::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (!DL->isCompleteDefinition())
      return;
    cur_classes.push_back(DL);
  } else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
    if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MT->getLocation()))
      return;

    if (MT->hasBody() && !MT->isVirtual()) {
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
      auto target = cur_classes[index];
      declaration.insert(declaration.rfind("}"),
                         "\n/*mut360*/" + target->getNameAsString() + "().{};");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
  }
}
  
void MutatorFrontendAction_360::MutatorASTConsumer_360::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
  auto class_matcher = cxxRecordDecl().bind("Classes");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(class_matcher, &callback);
  matchFinder.matchAST(Context);
}