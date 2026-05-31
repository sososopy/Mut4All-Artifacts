//source file
#include "../include/modify_module_related_declarations_442.h"

// ========================================================================================================
#define MUT442_OUTPUT 1

void MutatorFrontendAction_442::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("GlobalVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (!VD->isFileVarDecl() || VD->hasExternalStorage())
        return;

      std::string VarDeclText = Rewrite.getRewrittenText(VD->getSourceRange());

      if (VarDeclText.find("export") == std::string::npos) {
        std::string MutatedText = "export " + VarDeclText;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), MutatedText);
      }
    }
}
  
void MutatorFrontendAction_442::MutatorASTConsumer_442::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(isExpansionInMainFile(), hasGlobalStorage()).bind("GlobalVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}