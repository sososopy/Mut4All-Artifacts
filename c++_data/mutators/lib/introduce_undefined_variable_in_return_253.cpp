//source file
#include "../include/Introduce_Undefined_Variable_In_Return_253.h"

// ========================================================================================================
#define MUT253_OUTPUT 1

void MutatorFrontendAction_253::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("returnStmt")) {
        if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(RS->getBeginLoc()))
            return;

        if (auto *IF = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
            // Check if the return statement is in the else branch
            if (IF->getElse() && IF->getElse()->getStmtClass() == RS->getStmtClass()) {
                // Perform mutation by replacing the return statement with an undefined variable
                std::string undefinedVar = "undefinedVar";
                std::string replacement = "return " + undefinedVar + ";";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()), replacement);
            }
        }
    }
}

void MutatorFrontendAction_253::MutatorASTConsumer_253::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto ifMatcher = ifStmt(hasElse(returnStmt().bind("returnStmt"))).bind("ifStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(ifMatcher, &callback);
    matchFinder.matchAST(Context);
}