//source file
#include "../include/Replace_Template_Alias_With_Empty_Declaration_225.h"

// ========================================================================================================
#define MUT225_OUTPUT 1

void MutatorFrontendAction_225::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TemplateAlias")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (auto *T = dyn_cast<TypeAliasDecl>(MT)) {
        if (T->isTemplated() == false)
          return;
        auto alias =
            stringutils::rangetoStr(*(Result.SourceManager), T->getSourceRange());
        llvm::outs() << alias << '\n';
        alias = alias.substr(0, alias.find('='));
        llvm::outs() << alias << '\n';
        alias += " ;";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(T->getSourceRange()), alias);
      }
    }
}
  
void MutatorFrontendAction_225::MutatorASTConsumer_225::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl().bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}