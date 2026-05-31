//source file
#include "../include/Namespace_Alias_Misuse_207.h"

// ========================================================================================================
#define MUT207_OUTPUT 1

void MutatorFrontendAction_207::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      if (NS->isAnonymousNamespace())
        return;
      targetNamespace = NS;
      targetNamespaceName = NS->getNameAsString();
    } else if (auto *NG =
                   Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Guide")) {
      if (!NG || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NG->getBeginLoc()))
        return;
      if (!NG->isThisDeclarationADefinition())
        return;

      if (targetNamespace == nullptr || targetNamespaceName == "")
        return;
      auto aliasname = "mut207";
      auto aliasdecl = string("namespace ") + aliasname + " = " + targetNamespaceName;
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          NG->getSourceRange());
      llvm::outs() << decl << '\n';
      stringutils::strReplace(decl, targetNamespaceName, aliasname);
      llvm::outs() << decl << '\n';
      Rewrite.ReplaceText(NG->getBeginLoc(), 0, "/*mut207*/" + aliasdecl + ";\n");
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(NG->getSourceRange()), decl);
    }
}
  
void MutatorFrontendAction_207::MutatorASTConsumer_207::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto ns_matcher = namespaceDecl().bind("Namespace");
    auto guide_matcher = classTemplateDecl().bind("Guide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(ns_matcher, &callback);
    matchFinder.addMatcher(guide_matcher, &callback);
    matchFinder.matchAST(Context);
}