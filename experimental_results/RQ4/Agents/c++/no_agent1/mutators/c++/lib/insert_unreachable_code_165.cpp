//source file
#include "../include/insert_unreachable_code_165.h"

// ========================================================================================================
#define MUT165_OUTPUT 1

void MutatorFrontendAction_165::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto body = FS->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    body->getSourceRange());

            // Insert unreachable code
            std::string unreachableCode = "\n/*mut165*/if (false) { volatile int x = 0; x++; }\n";
            bodyText.insert(bodyText.find("{") + 1, unreachableCode);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_165::MutatorASTConsumer_165::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}