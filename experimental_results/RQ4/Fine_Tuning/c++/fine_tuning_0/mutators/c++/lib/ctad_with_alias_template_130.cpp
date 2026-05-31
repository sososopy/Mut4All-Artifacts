//source file
#include "../include/CTAD_with_alias_template_130.h"

// ========================================================================================================
#define MUT130_OUTPUT 1

void MutatorFrontendAction_130::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
    if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   AL->getLocation()))
      return;
    if (AL->isTransparentTag())
      return;
    cur_alias.push_back(AL);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (cur_alias.empty())
      return;
    auto target = cur_alias[getrandom::getRandomIndex(cur_alias.size() - 1)];
    auto ALname = target->getNameAsString();
    ALname += "(";
    int dice = getrandom::getRandomIndex(2);
    if (dice == 0)
      ALname += "1";
    else if (dice == 1)
      ALname += "nullptr";
    else
      ALname += "true";
    ALname += ")";
    Rewrite.ReplaceText(DL->getNameInfo().getSourceRange(), ALname);
  }
}
  
void MutatorFrontendAction_130::MutatorASTConsumer_130::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = typeAliasDecl().bind("Alias");
    auto deduction_matcher = cxxDeductionGuideDecl().bind("DL");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(deduction_matcher, &callback);
    matchFinder.matchAST(Context);
}