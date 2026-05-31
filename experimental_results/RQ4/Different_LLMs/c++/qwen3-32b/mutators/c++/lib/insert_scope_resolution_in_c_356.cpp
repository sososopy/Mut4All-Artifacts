//source file
#include "../include/Insert_Scope_Resolution_In_C_356.h"

// ========================================================================================================
#define MUT356_OUTPUT 1

void MutatorFrontendAction_356::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *S = Result.Nodes.getNodeAs<clang::Stmt>("stmt")) {
        // Check if in main file and is a C file
        if (!Result.Context->getSourceManager().isWrittenInMainFile(S->getBeginLoc())) {
            return;
        }
        auto SM = &Result.Context->getSourceManager();
        auto loc = S->getBeginLoc();
        if (SM->isInMainFile(loc)) {
            auto file = SM->getFilename(loc);
            if (!file.endswith(".c")) {
                return;
            }
        } else {
            return;
        }
        // Replace the statement with ::;
        auto range = S->getSourceRange();
        Rewrite.ReplaceText(range, "::;");
    }
}

void MutatorFrontendAction_356::MutatorASTConsumer_356::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = stmt().bind("stmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}