//source file
#include "../include/inject_nested_namespace_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NSD->getLocation()))
        return;

      std::string originalNamespace = stringutils::rangetoStr(*(Result.SourceManager), NSD->getSourceRange());
      
      // Perform mutation by injecting nested namespaces
      std::string nestedNamespace = "namespace B { namespace A { namespace C { ";
      std::string endBraces = "\n} } } // end nested namespaces\n";

      std::string mutatedNamespace = "/*mut28*/" + nestedNamespace + originalNamespace + endBraces;

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NSD->getSourceRange()), mutatedNamespace);
    }
}
  
void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl().bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}