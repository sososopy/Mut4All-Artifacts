//source file
#include "../include/incomplete_template_parameters_in_class_template_specialization_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Spec = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specialization")) {
      if (!Spec || !Result.Context->getSourceManager().isWrittenInMainFile(Spec->getLocation()))
        return;

      const TemplateArgumentList &Args = Spec->getTemplateArgs();
      if (Args.size() < 2) // Ensure there are at least two parameters
        return;

      std::string original = stringutils::rangetoStr(*(Result.SourceManager), Spec->getSourceRange());
      std::string mutated = original;

      // Perform mutation by removing the last template parameter
      size_t pos = mutated.rfind(',');
      if (pos != std::string::npos) {
        mutated = mutated.substr(0, pos) + ">";
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Spec->getSourceRange()), mutated);
    }
}
  
void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("Specialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}