//source file
#include "../include/misplace_template_argument_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateUsage")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getTemplateName().getAsTemplateDecl()->getLocation()))
        return;

      auto templateUsage = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getTemplateName().getAsTemplateDecl()->getLocation());
      
      size_t pos = templateUsage.find('>');
      if (pos != std::string::npos) {
        templateUsage.insert(pos, ">");
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getTemplateName().getAsTemplateDecl()->getLocation(), MT->getTemplateName().getAsTemplateDecl()->getLocation()), templateUsage);
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateSpecializationType().bind("TemplateUsage");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}