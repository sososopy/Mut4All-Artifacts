//source file
#include "../include/Modify_Integer_Literal_352.h"

// ========================================================================================================
#define MUT352_OUTPUT 1

void MutatorFrontendAction_352::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IL = Result.Nodes.getNodeAs<clang::IntegerLiteral>(("IntegerLiteral"))) {
      //Filter nodes in header files
      if (!IL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IL->getLocation()))
        return;
      //Get the source code text of target node
      std::string integerLiteral = stringutils::rangetoStr(*(Result.SourceManager), IL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      int value = std::stoi(integerLiteral);
      int mutatedValue = -value; // Change the sign of the integer literal
      std::string mutatedIntegerLiteral = std::to_string(mutatedValue);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(IL->getSourceRange()), mutatedIntegerLiteral);
    }
}

void MutatorFrontendAction_352::MutatorASTConsumer_352::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = integerLiteral().bind("IntegerLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}