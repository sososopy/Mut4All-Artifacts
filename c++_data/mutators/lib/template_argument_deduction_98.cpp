//source file
#include "../include/Template_Argument_Deduction_98.h"

// ========================================================================================================
#define MUT98_OUTPUT 1

void MutatorFrontendAction_98::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NTTP = Result.Nodes.getNodeAs<clang::TemplateArgumentLoc>("NonTypeTemplateParam")) {
      if (!NTTP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NTTP->getLocation()))
        return;

      auto originalArg = NTTP->getSourceRange();
      auto argText = stringutils::rangetoStr(*(Result.SourceManager), originalArg);

      if (argText == "5") {
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(originalArg), "10");
      }
    }
}
  
void MutatorFrontendAction_98::MutatorASTConsumer_98::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateArgumentLoc().bind("NonTypeTemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}