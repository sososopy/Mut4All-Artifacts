//source file
#include "../include/disrupt_namespace_nesting_28.h"

// ========================================================================================================
#define MUT28_OUTPUT 1

void MutatorFrontendAction_28::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      //Filter nodes in header files
      if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NSD->getLocation()))
        return;

      //Get the source code text of target node
      auto namespaceName = NSD->getNameAsString();
      auto range = NSD->getSourceRange();
      auto startLoc = range.getBegin();
      auto endLoc = range.getEnd();
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutation = "} ; namespace " + namespaceName + " { /*mut28*/";
      Rewrite.InsertTextAfterToken(endLoc, mutation);
    }
}
  
void MutatorFrontendAction_28::MutatorASTConsumer_28::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl(hasDescendant(namespaceDecl())).bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}