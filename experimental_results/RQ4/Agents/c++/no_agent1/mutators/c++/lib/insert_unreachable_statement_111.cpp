//source file
#include "../include/insert_unreachable_statement_111.h"

// ========================================================================================================
#define MUT111_OUTPUT 1

void MutatorFrontendAction_111::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        
        if (FD->hasBody()) {
            auto body = FD->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            
            // Insert an unreachable statement at the beginning of the function body
            std::string unreachableStmt = "\n/*mut111*/__builtin_unreachable();\n";
            bodyText.insert(bodyText.find("{") + 1, unreachableStmt);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_111::MutatorASTConsumer_111::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}