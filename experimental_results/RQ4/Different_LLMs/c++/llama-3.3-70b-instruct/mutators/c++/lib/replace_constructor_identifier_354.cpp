//source file
#include "../include/Replace_Constructor_Identifier_354.h"

// ========================================================================================================
#define MUT354_OUTPUT 1

void MutatorFrontendAction_354::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructExpr>(("Constructor"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Assuming we have a list of possible identifiers to replace with
      std::vector<std::string> possibleIdentifiers = {"orange", "apple", "banana"};
      int choice = getrandom::getRandomIndex(possibleIdentifiers.size() - 1);
      std::string newIdentifier = possibleIdentifiers[choice];
      size_t pos = declaration.find("banane");
      if (pos != std::string::npos) {
        declaration.replace(pos, 6, newIdentifier);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_354::MutatorASTConsumer_354::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxConstructExpr().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}