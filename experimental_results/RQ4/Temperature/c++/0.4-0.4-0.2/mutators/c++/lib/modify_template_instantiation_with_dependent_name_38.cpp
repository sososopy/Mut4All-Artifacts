//source file
#include "../include/modify_template_instantiation_with_dependent_name_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::DependentScopeDeclRefExpr>("templateInst")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      auto sourceText = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()),
          *Result.SourceManager, Result.Context->getLangOpts());

      std::string mutatedText = sourceText.str();
      size_t pos = mutatedText.find("template ");
      if (pos != std::string::npos) {
          mutatedText.erase(pos, 9); // Remove 'template' keyword
      }
      
      Rewrite.ReplaceText(MT->getSourceRange(), mutatedText);
    }
}
  
void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = declRefExpr(hasAncestor(functionTemplateDecl())).bind("templateInst");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}