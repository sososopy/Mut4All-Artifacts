//source file
#include "../include/Mutator_ConditionalStatementNegation_179.h"

// ========================================================================================================
#define MUT179_OUTPUT 1

void MutatorFrontendAction_179::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IfStmt = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!IfStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IfStmt->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = stringutils::rangetoStr(*(Result.SourceManager),
                                               IfStmt->getCond()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto mutatedCondition = "!(" + condition + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(IfStmt->getCond()->getSourceRange()), mutatedCondition);
    }
}
  
void MutatorFrontendAction_179::MutatorASTConsumer_179::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::ifStmt().bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}