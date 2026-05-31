//source file
#include "../include/insert_unreachable_code_block_496.h"

// ========================================================================================================
#define MUT496_OUTPUT 1

void MutatorFrontendAction_496::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto bodyRange = FS->getBody()->getSourceRange();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);

            // Insert an unreachable code block at the beginning of the function body
            std::string unreachableCode = "{\n/*mut496*/if (false) { int unreachable = 0; unreachable++; }\n";
            bodyText.insert(bodyText.find("{") + 1, unreachableCode);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
        }
    }
}

void MutatorFrontendAction_496::MutatorASTConsumer_496::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}