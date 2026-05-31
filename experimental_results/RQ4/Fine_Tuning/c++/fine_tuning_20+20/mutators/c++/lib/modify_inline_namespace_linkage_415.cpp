//source file
#include "../include/modify_inline_namespace_linkage_415.h"

// ========================================================================================================
#define MUT415_OUTPUT 1

void MutatorFrontendAction_415::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespaces")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       NS->getLocation()))
          return;
        if (NS->isInlineNamespace()) {
          target_ns = NS;
        }
      } else if (auto *DL = Result.Nodes.getNodeAs<clang::Decl>("Decls")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       DL->getLocation()))
          return;
        if (target_ns == nullptr) {
          if (DL->getKind() == Decl::Kind::TranslationUnit) {
            Rewrite.ReplaceText(DL->getEndLoc(), 0,
                                "\n/*mut415*/inline namespace mut_415{}");
          }
        } else {
          auto linkage = target_ns->getIdentifierNamespace();
          string linkage_str = "";
          if (linkage == 2)
            linkage_str = "inline";
          else if (linkage == 4)
            linkage_str = "internal";
          else if (linkage == 8)
            linkage_str = "external";
          else
            return;
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 target_ns->getSourceRange());
          llvm::outs() << content << '\n';
          if (content.find("inline") != string::npos)
            content.replace(content.find("inline"), 6, linkage_str);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                  target_ns->getSourceRange()),
                              "/*mut415*/" + content);
        }
      }
}
  
void MutatorFrontendAction_415::MutatorASTConsumer_415::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = namespaceDecl().bind("Namespaces");
    auto decl_matcher = decl().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}