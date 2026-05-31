//source file
#include "../include/Partial_Specialization_Of_Variable_Templates_With_Template_Template_Parameters_168.h"

// ========================================================================================================
#define MUT168_OUTPUT 1

void MutatorFrontendAction_168::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find("template <typename T, template <Ts...> class container> static constexpr bool value = false;") != string::npos) {
        content.insert(content.rfind('}'),
                       "/*mut168*/template <template <Ts...> class container, "
                       "Ts... params> static constexpr bool "
                       "value<container<params...>, container> = true;");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(DL->getSourceRange()), content);
      }
    }
}
  
void MutatorFrontendAction_168::MutatorASTConsumer_168::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}