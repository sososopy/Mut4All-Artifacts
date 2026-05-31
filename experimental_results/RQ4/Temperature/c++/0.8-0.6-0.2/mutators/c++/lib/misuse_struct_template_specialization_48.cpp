//source file
#include "../include/misuse_struct_template_specialization_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("StructSpecialization")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      auto specialization = MT->getSpecializedTemplate();
      if (!specialization)
        return;

      auto &SM = Rewrite.getSourceMgr();
      auto &LO = Rewrite.getLangOpts();
      auto specializationRange = MT->getSourceRange();
      std::string specializationText = Lexer::getSourceText(CharSourceRange::getTokenRange(specializationRange), SM, LO).str();

      std::string mutatedText = specializationText;
      size_t pos = mutatedText.find("::value");
      if (pos != std::string::npos) {
          mutatedText.replace(pos, 7, "::undefined_value");
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(specializationRange), mutatedText);
    }
}
  
void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(
        hasAncestor(classTemplateDecl(hasName("conditional_type")))
    ).bind("StructSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}