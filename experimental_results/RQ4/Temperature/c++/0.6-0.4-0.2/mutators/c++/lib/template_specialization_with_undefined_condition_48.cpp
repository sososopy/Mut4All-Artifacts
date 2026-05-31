//source file
#include "../include/Mutator_template_specialization_with_undefined_condition_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      for (auto *Spec : MT->specializations()) {
        if (Spec->isThisDeclarationADefinition()) {
          auto *ReturnType = Spec->getReturnType().getTypePtrOrNull();
          if (ReturnType && ReturnType->isDependentType()) {
            auto sourceRange = Spec->getSourceRange();
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

            size_t pos = declaration.find("enable_if<");
            if (pos != std::string::npos) {
              size_t endPos = declaration.find(",", pos);
              if (endPos != std::string::npos) {
                declaration.replace(pos, endPos - pos, "T::non_existent_member");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), declaration);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(expr(anyOf(cxxDependentScopeMemberExpr(), declRefExpr(to(namedDecl(hasName("enable_if")))))))).bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}