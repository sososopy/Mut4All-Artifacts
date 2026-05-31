//source file
#include "../include/add_unreachable_code_block_459.h"

// ========================================================================================================
#define MUT459_OUTPUT 1

void MutatorFrontendAction_459::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto body = FD->getBody();
            auto bodySourceRange = body->getSourceRange();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodySourceRange);

            std::string unreachableCode = "\n/*mut459*/if (false) {\n    int unreachable_var = 0;\n}\n";
            bodyText.insert(bodyText.find_last_of('}'), unreachableCode);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodySourceRange), bodyText);
        }
    }
}

void MutatorFrontendAction_459::MutatorASTConsumer_459::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}