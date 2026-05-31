//source file
#include "../include/Incomplete_Function_Body_268.h"

// ========================================================================================================
#define MUT268_OUTPUT 1

void MutatorFrontendAction_268::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Check if it's non-inline
        if (FD->isInlined())
            return;

        // Get the body
        Stmt *Body = FD->getBody();
        if (!Body)
            return;

        // Replace the body with a single '{'
        CharSourceRange BodyRange = CharSourceRange::getTokenRange(Body->getSourceRange());
        Rewrite.ReplaceText(BodyRange, "{");
    }
}

void MutatorFrontendAction_268::MutatorASTConsumer_268::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(stmt()), unless(isInline())).bind("function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}