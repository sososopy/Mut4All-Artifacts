//source file
#include "../include/Insert_Invalid_Namespace_Usage_271.h"

// ========================================================================================================
#define MUT271_OUTPUT 1

void MutatorFrontendAction_271::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespaces")) {
      //Filter nodes in header files
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   NS->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string invalidNamespaceUsage = "\nnamespace using ::memmove;\n";
      declaration.insert(declaration.find("}"), invalidNamespaceUsage);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NS->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_271::MutatorASTConsumer_271::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl().bind("Namespaces");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}