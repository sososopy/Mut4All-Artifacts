//source file
#include "../include/remove_template_type_qualifier_632.h"

// ========================================================================================================
#define MUT632_OUTPUT 1

void MutatorFrontendAction_632::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;

      auto typeSourceInfo = TA->getTypeSourceInfo();
      if (!typeSourceInfo)
        return;

      auto typeLoc = typeSourceInfo->getTypeLoc();
      auto typeRange = typeLoc.getSourceRange();
      std::string typeText = Lexer::getSourceText(CharSourceRange::getTokenRange(typeRange), 
                                                  Result.Context->getSourceManager(), 
                                                  Result.Context->getLangOpts()).str();

      std::string mutatedTypeText = typeText;
      size_t pos = mutatedTypeText.find("_Nullable");
      if (pos != std::string::npos) {
          mutatedTypeText.erase(pos, std::string("_Nullable").length());
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(typeRange), mutatedTypeText);
    }
}
  
void MutatorFrontendAction_632::MutatorASTConsumer_632::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl(hasType(asString("_Nullable"))).bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}