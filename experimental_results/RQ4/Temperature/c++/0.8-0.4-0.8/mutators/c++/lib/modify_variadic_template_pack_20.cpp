//source file
#include "../include/modify_variadic_template_pack_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("VariadicFunction")) {
      if (!FDecl || !Result.Context->getSourceManager().isWrittenInMainFile(FDecl->getLocation()))
        return;

      if (FDecl->isVariadic()) {
        auto body = FDecl->getBody();
        if (!body)
          return;

        auto functionSource = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
        size_t pos = functionSource.find("...");

        if (pos != std::string::npos) {
          functionSource.erase(pos, 3);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), functionSource);
        }
      }
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isVariadic()).bind("VariadicFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}