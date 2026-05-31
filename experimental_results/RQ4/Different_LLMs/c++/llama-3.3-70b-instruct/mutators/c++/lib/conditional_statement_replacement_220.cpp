//source file
#include "../include/Mutator_Conditional_Statement_Replacement_220.h"

// ========================================================================================================
#define MUT220_OUTPUT 1

void MutatorFrontendAction_220::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!MT ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto condition = MT->getCond();
      if (!condition)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string newCondition = "x > 0"; // example of a new condition
      llvm::StringRef conditionStr = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(condition->getSourceRange()), 
          *Result.SourceManager, 
          clang::LangOptions()
      );
      std::string newConditionStr = "/*mut220*/" + newCondition;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(condition->getSourceRange()), newConditionStr);
    }
}
  
void MutatorFrontendAction_220::MutatorASTConsumer_220::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::StatementMatcher matcher = clang::ast_matchers::ifStmt().bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}