//source file
#include "../include/misplaced_semicolon_in_nested_namespaces_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;

      if (NS->isAnonymousNamespace() || NS->isInline())
        return;

      auto startLoc = NS->getBeginLoc();
      auto endLoc = NS->getRBraceLoc();
      if (startLoc.isInvalid() || endLoc.isInvalid())
        return;

      auto sourceRange = clang::SourceRange(startLoc, endLoc);
      auto text = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      size_t pos = text.find('{');
      if (pos != std::string::npos) {
        text.insert(pos, ";");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), text);
      }
    }
}

void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(has(namespaceDecl())).bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}