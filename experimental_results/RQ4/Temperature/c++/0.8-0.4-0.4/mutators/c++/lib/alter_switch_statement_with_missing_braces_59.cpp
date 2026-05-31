//source file
#include "../include/alter_switch_statement_with_missing_braces_59.h"

// ========================================================================================================
#define MUT59_OUTPUT 1

void MutatorFrontendAction_59::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SC = Result.Nodes.getNodeAs<clang::SwitchCase>("SwitchCase")) {
        if (!SC || !Result.Context->getSourceManager().isWrittenInMainFile(SC->getBeginLoc()))
            return;

        if (const Stmt *SubStmt = SC->getSubStmt()) {
            SourceLocation StartLoc = SubStmt->getBeginLoc();
            SourceLocation EndLoc = SubStmt->getEndLoc();

            if (StartLoc.isValid() && EndLoc.isValid() && !isa<CompoundStmt>(SubStmt)) {
                std::string OriginalText = Lexer::getSourceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), 
                                                                *Result.SourceManager, Result.Context->getLangOpts()).str();
                std::string MutatedText = "{ " + OriginalText + " }";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), MutatedText);
            }
        }
    }
}

void MutatorFrontendAction_59::MutatorASTConsumer_59::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = switchCase().bind("SwitchCase");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}