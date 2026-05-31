//source file
#include "../include/template_specialization_constructor_mutation_367.h"

// ========================================================================================================
#define MUT367_OUTPUT 1

void MutatorFrontendAction_367::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;

      for (auto *spec : CTD->specializations()) {
        if (spec->isCompleteDefinition()) {
          auto className = spec->getNameAsString();
          classNames[spec] = className;
          auto constructor = spec->getDefinition()->getFirstDecl();
          if (constructor) {
            auto constructorRange = constructor->getSourceRange();
            auto constructorText = stringutils::rangetoStr(*(Result.SourceManager), constructorRange);
            constructorText.insert(constructorText.find(")"), ", int additionalParam");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(constructorRange), constructorText);
          }
        }
      }
    } else if (auto *CXXRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("InstantiatedClass")) {
      if (!CXXRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CXXRD->getLocation()))
        return;

      auto className = CXXRD->getNameAsString();
      if (classNames.find(CXXRD) != classNames.end()) {
        auto instantiationText = classNames[CXXRD] + "<char, int> instance('a', 42);";
        Rewrite.InsertTextAfterToken(CXXRD->getEndLoc(), "\n/*mut367*/" + instantiationText);
      }
    }
}
  
void MutatorFrontendAction_367::MutatorASTConsumer_367::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto templateMatcher = classTemplateDecl(has(cxxRecordDecl(has(cxxConstructorDecl())))).bind("TemplateClass");
    auto instantiationMatcher = cxxRecordDecl().bind("InstantiatedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(instantiationMatcher, &callback);
    matchFinder.matchAST(Context);
}