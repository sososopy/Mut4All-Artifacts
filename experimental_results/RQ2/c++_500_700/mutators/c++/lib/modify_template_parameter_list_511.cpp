//source file
#include "../include/modify_template_parameter_list_511.h"

// ========================================================================================================
#define MUT511_OUTPUT 1

void MutatorFrontendAction_511::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      auto templateParams = FTD->getTemplateParameters();
      if (templateParams->size() < 2)
        return;

      auto sourceRange = FTD->getSourceRange();
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      size_t pos = declaration.find('>');
      if (pos != std::string::npos) {
        declaration.insert(pos, ", template<typename> class T");
        pos = declaration.find('{');
        if (pos != std::string::npos) {
          declaration.insert(pos + 1, "\nusing NewType = T<Base>;\nstd::make_unique<NewType>();\n");
        }
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), declaration);
      }
    }
}
  
void MutatorFrontendAction_511::MutatorASTConsumer_511::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        has(templateTypeParmDecl()), has(templateTemplateParmDecl())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}