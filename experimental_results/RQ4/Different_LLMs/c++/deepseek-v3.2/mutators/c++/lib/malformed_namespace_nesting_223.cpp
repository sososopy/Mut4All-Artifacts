//source file
#include "../include/Malformed_Namespace_Nesting_223.h"

// ========================================================================================================
#define MUT223_OUTPUT 1

void MutatorFrontendAction_223::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NamespaceDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Check if it's a nested namespace (contains ::)
      size_t namespacePos = declaration.find("namespace ");
      if (namespacePos == std::string::npos) return;
      
      size_t startPos = namespacePos + 10; // after "namespace "
      size_t endPos = declaration.find("{", startPos);
      if (endPos == std::string::npos) return;
      
      std::string namespacePart = declaration.substr(startPos, endPos - startPos);
      
      // Check for nested namespace syntax (::)
      size_t colonColonPos = namespacePart.find("::");
      if (colonColonPos != std::string::npos) {
        // Nested namespace: insert semicolon after one of the identifiers
        // Choose random position among the identifiers
        std::vector<size_t> identifierPositions;
        size_t currentPos = 0;
        while (currentPos < namespacePart.length()) {
          size_t nextColon = namespacePart.find("::", currentPos);
          if (nextColon == std::string::npos) {
            identifierPositions.push_back(currentPos);
            break;
          }
          identifierPositions.push_back(currentPos);
          currentPos = nextColon + 2;
        }
        
        if (identifierPositions.empty()) return;
        
        size_t chosenIdx = getrandom::getRandomIndex(identifierPositions.size() - 1);
        size_t insertPos = startPos + identifierPositions[chosenIdx];
        
        // Find the end of this identifier
        size_t identifierEnd = namespacePart.find("::", identifierPositions[chosenIdx]);
        if (identifierEnd == std::string::npos) {
          identifierEnd = namespacePart.length();
        } else {
          identifierEnd += identifierPositions[chosenIdx];
        }
        
        // Insert "; " after the identifier
        declaration.insert(startPos + identifierEnd, "; ");
      } else {
        // Simple namespace: insert semicolon between namespace keyword and identifier
        declaration.insert(startPos, "; ");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_223::MutatorASTConsumer_223::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl().bind("NamespaceDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}