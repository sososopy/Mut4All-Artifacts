//source file
#include "../include/modify_template_specialization_enable_if_578.h"

// ========================================================================================================
#define MUT578_OUTPUT 1

void MutatorFrontendAction_578::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithEnableIf")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      auto funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      std::string newSpec = "template<typename T, typename = typename std::enable_if<!std::is_integral<T>::value>::type>";

      size_t pos = funcText.find("template<");
      if (pos != std::string::npos) {
        funcText.insert(pos, newSpec);
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
    }
}
  
void MutatorFrontendAction_578::MutatorASTConsumer_578::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasTemplateArgument(0, refersToType(hasDeclaration(namedDecl(hasName("enable_if")))))).bind("FunctionWithEnableIf");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}