//source file
#include "../include/Add_Requires_To_Primary_Template_182.h"

// ========================================================================================================
#define MUT182_OUTPUT 1

void MutatorFrontendAction_182::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      cur_templates.push_back(TD);
    } else if (auto *SD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>(
                   "Specializations")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      cur_specializations.push_back(SD);
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::Expr>(
                   "RequiresClauses")) {
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getBeginLoc()))
        return;
      if (cur_templates.empty() || cur_specializations.empty())
        return;
      auto TD = cur_templates.back();
      auto SD = cur_specializations.back();
      if (SD->getSpecializationKind() !=
          TemplateSpecializationKind::TSK_ExplicitSpecialization)
        return;
      auto primary = TD->getTemplatedDecl();
      auto partial = SD->getSpecializedTemplateOrPartial();
      if (!partial.is<clang::ClassTemplateDecl*>())
        return;
      auto partial_decl = partial.get<clang::ClassTemplateDecl*>();
      if (primary != partial_decl->getTemplatedDecl())
        return;
      auto requires_clause = stringutils::rangetoStr(
          *(Result.SourceManager), RD->getSourceRange());
      llvm::outs() << requires_clause << '\n';
      auto primary_decl = stringutils::rangetoStr(
          *(Result.SourceManager), primary->getSourceRange());
      auto partial_decl_str = stringutils::rangetoStr(
          *(Result.SourceManager), SD->getSourceRange());
      primary_decl.insert(primary_decl.find_last_of('>') + 1,
                          " /*mut182*/" + requires_clause);
      partial_decl_str.insert(partial_decl_str.find_last_of('>') + 1,
                          " /*mut182*/" + requires_clause);
      llvm::outs() << primary_decl << '\n';
      llvm::outs() << partial_decl_str << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(primary->getSourceRange()),
                          primary_decl);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()),
                          partial_decl_str);
    }
  }
  
void MutatorFrontendAction_182::MutatorASTConsumer_182::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto specialization_matcher =
        classTemplateSpecializationDecl().bind("Specializations");
    auto requires_matcher = expr().bind("RequiresClauses");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(specialization_matcher, &callback);
    matchFinder.addMatcher(requires_matcher, &callback);
    matchFinder.matchAST(Context);
}