//source file
#include "../include/manipulate_template_function_argument_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunc")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isConstexpr()) {
        auto templateArgs = MT->getTemplateSpecializationArgs();
        if (templateArgs && templateArgs->size() > 0) {
          auto sourceRange = MT->getSourceRange();
          auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
          auto pos = sourceText.find('<');
          if (pos != std::string::npos) {
            sourceText.replace(pos, sourceText.find('>', pos) - pos + 1, "<InvalidType>");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isConstexpr()).bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}