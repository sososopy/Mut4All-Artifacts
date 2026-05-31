//source file
#include "../include/Insert_Nested_Block_493.h"

// ========================================================================================================
#define MUT493_OUTPUT 1

void MutatorFrontendAction_493::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
        if (!FD || !Result.SourceManager->isWrittenInMainFile(FD->getLocation()))
            return;
        if (auto *CS = Result.Nodes.getNodeAs<clang::CompoundStmt>("compoundStmt")) {
            if (CS->size() < 1)
                return;
            const clang::Stmt *FirstStmt = CS->body_front();
            clang::SourceRange FirstStmtRange = FirstStmt->getSourceRange();
            std::string FirstStmtText = stringutils::rangetoStr(*Result.SourceManager, FirstStmtRange);
            std::string NewText = "{" + FirstStmtText + "}";
            Rewrite.ReplaceText(FirstStmtRange, NewText);
        }
    }
}
  
void MutatorFrontendAction_493::MutatorASTConsumer_493::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt().bind("compoundStmt"))).bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}