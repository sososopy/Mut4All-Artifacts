//source file
#include "../include/insert_unreachable_code_block_368.h"

// ========================================================================================================
#define MUT368_OUTPUT 1

void MutatorFrontendAction_368::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto body = FS->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

            std::string unreachableCode = "\n/*mut368*/if (false) {\n    int unreachableVar = 0;\n    unreachableVar++;\n}\n";

            bodyText.insert(bodyText.find("{") + 1, unreachableCode);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_368::MutatorASTConsumer_368::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}