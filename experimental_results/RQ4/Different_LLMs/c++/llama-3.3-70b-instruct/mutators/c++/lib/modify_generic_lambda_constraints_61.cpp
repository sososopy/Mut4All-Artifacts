//source file
#include "../include/Modify_Generic_Lambda_Constraints_61.h"

// ========================================================================================================
#define MUT61_OUTPUT 1

void MutatorFrontendAction_61::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                 LT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string modifiedText = lambdaText;
      size_t sameAsPos = modifiedText.find("std::same_as");
      if (sameAsPos != std::string::npos) {
        // Replace std::same_as with other concepts
        std::vector<std::string> replacementConcepts = {"std::convertible_to", "std::derived_from", "std::integral"};
        int choice = getrandom::getRandomIndex(replacementConcepts.size());
        modifiedText.replace(sameAsPos, 11, replacementConcepts[choice]);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), modifiedText);
    }
}
  
void MutatorFrontendAction_61::MutatorASTConsumer_61::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}