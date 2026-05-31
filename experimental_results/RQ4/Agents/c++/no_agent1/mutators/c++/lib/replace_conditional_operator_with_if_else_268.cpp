//source file
#include "../include/replace_conditional_operator_with_if_else_268.h"

// ========================================================================================================
#define MUT268_OUTPUT 1

void MutatorFrontendAction_268::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CO = Result.Nodes.getNodeAs<clang::ConditionalOperator>("CondOp")) {
        if (!CO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CO->getBeginLoc()))
            return;

        SourceManager &SM = Rewrite.getSourceMgr();
        LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        std::string condition = Lexer::getSourceText(CharSourceRange::getTokenRange(CO->getCond()->getSourceRange()), SM, LangOpts).str();
        std::string trueExpr = Lexer::getSourceText(CharSourceRange::getTokenRange(CO->getTrueExpr()->getSourceRange()), SM, LangOpts).str();
        std::string falseExpr = Lexer::getSourceText(CharSourceRange::getTokenRange(CO->getFalseExpr()->getSourceRange()), SM, LangOpts).str();

        std::string ifElseCode = "/*mut268*/if (" + condition + ") { " + trueExpr + "; } else { " + falseExpr + "; }";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CO->getSourceRange()), ifElseCode);
    }
}

void MutatorFrontendAction_268::MutatorASTConsumer_268::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conditionalOperator().bind("CondOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}