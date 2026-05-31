//source file
#include "../include/mutator_148.h"

// ========================================================================================================
#define MUT148_OUTPUT 1

void MutatorFrontendAction_148::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      namespaces.push_back(NS);
    } else if (auto *TU =
                   Result.Nodes.getNodeAs<clang::TranslationUnitDecl>(
                       "TranslationUnit")) {
      if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getBeginLoc()))
        return;
      if (namespaces.empty())
        return;
      size_t index = getrandom::getRandomIndex(namespaces.size() - 1);
      auto target = namespaces[index];
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             target->getSourceRange());
      int dice = getrandom::getRandomIndex(4);
      if (dice == 0) {
        if (target->getNameAsString() == "")
          return;
        content = "namespace " + target->getNameAsString() + " {\n" + content;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                target->getSourceRange()),
                            content);
      } else if (dice == 1) {
        if (content.find('}') != string::npos)
          content.erase(content.rfind('}'));
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                target->getSourceRange()),
                            content);
      } else if (dice == 2) {
        if (index + 1 == namespaces.size())
          return;
        auto next = namespaces[index + 1];
        auto next_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    next->getSourceRange());
        content += next_content;
        content = "namespace " + target->getNameAsString() + " {\n" + content;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                target->getSourceRange()),
                            content);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                next->getSourceRange()),
                            "");
      } else if (dice == 3) {
        if (index == 0)
          return;
        auto prev = namespaces[index - 1];
        auto prev_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    prev->getSourceRange());
        content = prev_content + content;
        content = "namespace " + prev->getNameAsString() + " {\n" + content;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                prev->getSourceRange()),
                            content);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                target->getSourceRange()),
                            "");
      } else if (dice == 4) {
        content = "namespace mut148 {\n" + content;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                target->getSourceRange()),
                            content);
      }
    }
}
  
void MutatorFrontendAction_148::MutatorASTConsumer_148::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = namespaceDecl().bind("Namespace");
    auto tu_matcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(tu_matcher, &callback);
    matchFinder.matchAST(Context);
}