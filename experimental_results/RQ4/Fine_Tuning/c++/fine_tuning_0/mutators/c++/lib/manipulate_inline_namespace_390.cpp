//source file
#include "../include/Manipulate_Inline_Namespace_390.h"

// ========================================================================================================
#define MUT390_OUTPUT 1

void MutatorFrontendAction_390::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::NamespaceDecl>("InlineNamespace")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto ns = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      llvm::outs() << ns << '\n';
      //Perform mutation on the source code text by applying string replacement
      int dice = getrandom::getRandomIndex(1);
      if (dice) {
        ns = "namespace mut_390 {\n" + ns + "\n}";
      } else {
        ns = "static " + ns;
      }
      ns = "/*mut390*/" + ns;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), ns);
    }
}
  
void MutatorFrontendAction_390::MutatorASTConsumer_390::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl(isInline()).bind("InlineNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}