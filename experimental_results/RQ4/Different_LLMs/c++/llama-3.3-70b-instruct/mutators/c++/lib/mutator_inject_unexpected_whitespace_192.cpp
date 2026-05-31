//source file
#include "../include/Mutator_Inject_Unexpected_Whitespace_192.h"

// ========================================================================================================
#define MUT192_OUTPUT 1

void MutatorFrontendAction_192::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::BinaryOperator>(("BinaryOperator"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      int choice = getrandom::getRandomIndex(1);
      if (choice) {
        // Remove whitespace around operator
        declaration.erase(std::remove(declaration.begin(), declaration.end(), ' '), declaration.end());
      } else {
        // Insert extra whitespace around operator
        declaration.insert(declaration.find_first_of("+*-/%"), "   ");
        declaration.insert(declaration.find_last_of("+*-/%") + 1, "   ");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_192::MutatorASTConsumer_192::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator().bind("BinaryOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}