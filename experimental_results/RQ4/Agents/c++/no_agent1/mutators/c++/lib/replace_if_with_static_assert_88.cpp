//source file
#include "../include/replace_if_with_static_assert_88.h"

// ========================================================================================================
#define MUT88_OUTPUT 1

void MutatorFrontendAction_88::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IF = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!IF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IF->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto condition = IF->getCond();
      if (!condition)
        return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string conditionText = Lexer::getSourceText(CharSourceRange::getTokenRange(condition->getSourceRange()), 
                                                       *Result.SourceManager, 
                                                       Result.Context->getLangOpts()).str();
      std::string staticAssertText = "static_assert(" + conditionText + ", \"Static assert triggered by mutator\");";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(IF->getSourceRange()), staticAssertText);
    }
}

void MutatorFrontendAction_88::MutatorASTConsumer_88::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt().bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}