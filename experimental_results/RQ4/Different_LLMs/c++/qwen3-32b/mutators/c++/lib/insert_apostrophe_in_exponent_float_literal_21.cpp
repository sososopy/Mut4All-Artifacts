//source file
#include "../include/Insert_Apostrophe_In_Exponent_Float_Literal_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *floatLit = Result.Nodes.getNodeAs<clang::FloatingLiteral>("floatLit")) {
        if (!floatLit || !Result.Context->getSourceManager().isWrittenInMainFile(floatLit->getLocation()))
            return;

        SourceManager &SM = *Result.SourceManager;
        auto range = floatLit->getSourceRange();
        std::string originalText = stringutils::rangetoStr(SM, range);

        size_t ePos = originalText.find('e');
        if (ePos == std::string::npos)
            ePos = originalText.find('E');
        if (ePos != std::string::npos) {
            std::string mutatedText = originalText.substr(0, ePos) + "'" + originalText.substr(ePos);
            Rewrite.ReplaceText(range, mutatedText);
        }
    }
}

void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    clang::ast_matchers::StatementMatcher matcher = clang::ast_matchers::floatLiteral().bind("floatLit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}