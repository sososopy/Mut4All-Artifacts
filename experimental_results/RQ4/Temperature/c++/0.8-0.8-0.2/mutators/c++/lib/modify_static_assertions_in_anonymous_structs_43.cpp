//source file
#include "../include/modify_static_assertions_in_anonymous_structs_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;

      auto sourceMgr = Result.SourceManager;
      auto originalText = stringutils::rangetoStr(*sourceMgr,
                                                  SA->getSourceRange());

      size_t start = originalText.find("__builtin_types_compatible_p(");
      size_t end = originalText.find("))", start);

      if (start != std::string::npos && end != std::string::npos) {
        std::string mutatedText = originalText;
        size_t insertPos = originalText.find_last_of(',', end);
        if (insertPos != std::string::npos) {
          std::string replacementType = "float*";
          mutatedText.replace(insertPos + 1, end - insertPos - 1, " " + replacementType);
        }
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SA->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}