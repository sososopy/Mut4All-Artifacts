//source file
#include "../include/replace_if_with_conditional_operator_304.h"

// ========================================================================================================
#define MUT304_OUTPUT 1

void MutatorFrontendAction_304::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *IF = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt")) {
        if (!IF || !Result.Context->getSourceManager().isWrittenInMainFile(IF->getIfLoc()))
            return;

        if (auto *Cond = dyn_cast<Expr>(IF->getCond())) {
            if (auto *Then = dyn_cast<ReturnStmt>(IF->getThen())) {
                if (auto *Else = dyn_cast<ReturnStmt>(IF->getElse())) {
                    auto CondText = Lexer::getSourceText(CharSourceRange::getTokenRange(Cond->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
                    auto ThenText = Lexer::getSourceText(CharSourceRange::getTokenRange(Then->getRetValue()->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
                    auto ElseText = Lexer::getSourceText(CharSourceRange::getTokenRange(Else->getRetValue()->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());

                    std::string TernaryExpr = "return " + CondText.str() + " ? " + ThenText.str() + " : " + ElseText.str() + ";";
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(IF->getSourceRange()), TernaryExpr);
                }
            }
        }
    }
}

void MutatorFrontendAction_304::MutatorASTConsumer_304::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = ifStmt(hasThen(returnStmt()), hasElse(returnStmt())).bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}