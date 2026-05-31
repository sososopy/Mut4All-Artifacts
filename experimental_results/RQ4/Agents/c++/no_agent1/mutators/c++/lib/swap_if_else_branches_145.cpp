//source file
#include "../include/swap_if_else_branches_145.h"

// ========================================================================================================
#define MUT145_OUTPUT 1

void MutatorFrontendAction_145::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *IfStmt = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
        if (!IfStmt || !Result.Context->getSourceManager().isWrittenInMainFile(
                        IfStmt->getIfLoc()))
            return;

        // Check if there is an else branch
        if (const Stmt *ElseStmt = IfStmt->getElse()) {
            // Get the source code text of the if and else branches
            auto IfBranchText = stringutils::rangetoStr(*(Result.SourceManager),
                                                        IfStmt->getThen()->getSourceRange());
            auto ElseBranchText = stringutils::rangetoStr(*(Result.SourceManager),
                                                          ElseStmt->getSourceRange());

            // Perform mutation by swapping the branches
            std::string MutatedIfStmt = "if (" + stringutils::rangetoStr(*(Result.SourceManager), 
                                    IfStmt->getCond()->getSourceRange()) + ") " + ElseBranchText + " else " + IfBranchText;

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(IfStmt->getSourceRange()), MutatedIfStmt);
        }
    }
}

void MutatorFrontendAction_145::MutatorASTConsumer_145::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = ifStmt(hasElse(stmt())).bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}