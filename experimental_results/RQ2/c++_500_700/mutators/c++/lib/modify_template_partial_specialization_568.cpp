//source file
#include "../include/Mutator_modify_template_partial_specialization_568.h"

// ========================================================================================================
#define MUT568_OUTPUT 1

void MutatorFrontendAction_568::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("TemplateSpecialization")) {
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(TS->getLocation()))
        return;

      auto specialization = stringutils::rangetoStr(*(Result.SourceManager), TS->getSourceRange());
      size_t pos = specialization.find_last_of('>');
      if (pos != std::string::npos) {
        specialization.insert(pos, ", typename Extra");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getSourceRange()), specialization);
    }
}
  
void MutatorFrontendAction_568::MutatorASTConsumer_568::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}