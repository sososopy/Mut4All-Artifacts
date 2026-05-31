//source file
#include "../include/swap_if_else_branches_55.h"

// ========================================================================================================
#define MUT55_OUTPUT 1

void MutatorFrontendAction_55::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IE = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
      //Filter nodes in header files
      if (!IE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IE->getBeginLoc()))
        return;

      // Ensure the if statement has both 'then' and 'else' branches
      if (const Stmt *ThenStmt = IE->getThen(); const Stmt *ElseStmt = IE->getElse()) {
        // Get the source code text of target node
        auto ThenRange = ThenStmt->getSourceRange();
        auto ElseRange = ElseStmt->getSourceRange();
        
        std::string ThenText = stringutils::rangetoStr(*(Result.SourceManager), ThenRange);
        std::string ElseText = stringutils::rangetoStr(*(Result.SourceManager), ElseRange);

        // Perform mutation on the source code text by swapping 'then' and 'else' branches
        std::string MutatedIfStmt = "/*mut55*/if" + 
                                    stringutils::rangetoStr(*(Result.SourceManager), IE->getCond()->getSourceRange()) +
                                    " " + ElseText + " else " + ThenText;

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(IE->getSourceRange()), MutatedIfStmt);
      }
    }
}

void MutatorFrontendAction_55::MutatorASTConsumer_55::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt(hasElse(anything())).bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}