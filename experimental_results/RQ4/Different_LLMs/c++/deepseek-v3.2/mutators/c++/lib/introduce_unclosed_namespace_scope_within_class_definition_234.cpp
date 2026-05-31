//source file
#include "../include/Introduce_Unclosed_Namespace_Scope_Within_Class_Definition_234.h"

// ========================================================================================================
#define MUT234_OUTPUT 1

void MutatorFrontendAction_234::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (!MT->isStruct() && !MT->isClass() && !MT->isUnion())
        return;
      //Get the source code text of target node
      auto classSourceRange = MT->getSourceRange();
      auto classSourceText = stringutils::rangetoStr(*(Result.SourceManager), classSourceRange);
      //Find the opening brace of the class body
      size_t openBracePos = classSourceText.find('{');
      if (openBracePos == std::string::npos)
        return;
      //Determine namespace identifier
      std::string namespaceIdentifier = "MutatedNS";
      if (!existingNamespaceNames.empty()) {
        namespaceIdentifier = existingNamespaceNames[getrandom::getRandomIndex(existingNamespaceNames.size() - 1)];
      }
      //Perform mutation on the source code text by applying string replacement
      std::string namespaceInsertion = "\nnamespace " + namespaceIdentifier + " {\n";
      //Insert after the opening brace
      classSourceText.insert(openBracePos + 1, namespaceInsertion);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(classSourceRange), classSourceText);
    }
    else if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NamespaceDecls")) {
      //Collect existing namespace names for potential reuse
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      std::string nsName = NS->getNameAsString();
      if (!nsName.empty()) {
        existingNamespaceNames.push_back(nsName);
      }
    }
}
  
void MutatorFrontendAction_234::MutatorASTConsumer_234::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("Classes");
    DeclarationMatcher namespaceMatcher = namespaceDecl().bind("NamespaceDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(namespaceMatcher, &callback);
    matchFinder.matchAST(Context);
}