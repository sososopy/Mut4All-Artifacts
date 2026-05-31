//source file
#include "../include/InsertInvalidOperatorAtEndOfDeclaration_313.h"

// ========================================================================================================
#define MUT313_OUTPUT 1

void MutatorFrontendAction_313::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::Decl>("VarDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::vector<std::string> invalidOperators = {"&=", "*=", "+=", "-=", "!=", "/="};
      int choice = getrandom::getRandomIndex(invalidOperators.size() - 1);
      declaration.insert(declaration.find(";"), " " + invalidOperators[choice] + " 0");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    } else if (auto *MT = Result.Nodes.getNodeAs<clang::Decl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::vector<std::string> invalidOperators = {"&=", "*=", "+=", "-=", "!=", "/="};
      int choice = getrandom::getRandomIndex(invalidOperators.size() - 1);
      declaration.insert(declaration.find(";"), " " + invalidOperators[choice] + " 0");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_313::MutatorASTConsumer_313::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl().bind("VarDecl");
    DeclarationMatcher funcMatcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}