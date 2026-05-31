//source file
#include "../include/insert_unreachable_code_245.h"

// ========================================================================================================
#define MUT245_OUTPUT 1

void MutatorFrontendAction_245::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            functions.push_back(FD);
        }
    } else if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnStmt")) {
        if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RS->getBeginLoc()))
            return;

        if (!functions.empty()) {
            auto func = functions.back();
            functions.pop_back();
            auto funcName = func->getNameAsString();
            std::string unreachableCode = "\n/*mut245*/if (false) { " + funcName + "(); }\n";
            SourceLocation insertLoc = RS->getBeginLoc();
            Rewrite.InsertTextBefore(insertLoc, unreachableCode);
        }
    }
}

void MutatorFrontendAction_245::MutatorASTConsumer_245::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl(hasBody(stmt())).bind("Functions");
    StatementMatcher returnStmtMatcher = returnStmt().bind("ReturnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(returnStmtMatcher, &callback);
    matchFinder.matchAST(Context);
}