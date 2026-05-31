//source file
#include "../include/enhance_template_recursion_156.h"

// ========================================================================================================
#define MUT156_OUTPUT 1

void MutatorFrontendAction_156::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (content.find("return") == string::npos)
        return;
      if (content.find("return 1") != string::npos)
        return;
      if (content.find("return 0") == string::npos)
        return;
      cur_templates.push_back(MT);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "Functions")) { // If there is a return 0 statement in the function, replace the return value with the inserted static const
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      if (!FD->isTemplateInstantiation())
        return;
      auto target = FD->getPrimaryTemplate();
      for (auto template_decl : cur_templates) {
        if (target == template_decl) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());
          if (content.find("return 0") == string::npos)
            return;
          content = std::regex_replace(content, std::regex("return 0"),
                                       "return 1");
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), content);
        }
      }
    }
}
  
void MutatorFrontendAction_156::MutatorASTConsumer_156::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("Templates");
    auto function_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.matchAST(Context);
}