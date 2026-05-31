//source file
#include "../include/invalid_type_qualifier_in_template_alias_315.h"

// ========================================================================================================
#define MUT315_OUTPUT 1

void MutatorFrontendAction_315::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("Aliases")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_alias.push_back(DL);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::TypeAliasDecl>("AliasDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (cur_alias.empty())
        return;
      auto target = cur_alias.back();
      if (DL->getNameAsString() != target->getNameAsString())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      content += " _Nullable";
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_315::MutatorASTConsumer_315::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = typeAliasTemplateDecl().bind("Aliases");
    auto alias_decl_matcher = typeAliasDecl().bind("AliasDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(alias_decl_matcher, &callback);
    matchFinder.matchAST(Context);
}