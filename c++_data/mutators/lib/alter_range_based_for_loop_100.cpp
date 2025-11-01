//source file
#include "../include/Alter_Range_Based_For_Loop_10000.h"

// ========================================================================================================
#define MUT100_OUTPUT 1

void MutatorFrontendAction_10000::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FL = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("ForLoop")) {
        if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FL->getForLoc()))
            return;

        auto &SM = Result.Context->getSourceManager();
        auto &LO = Result.Context->getLangOpts();
        auto LoopRange = FL->getSourceRange();
        auto LoopText = Lexer::getSourceText(CharSourceRange::getTokenRange(LoopRange), SM, LO);

        std::string condition = "true";
        std::string value1 = "1";
        std::string value2 = "2";

        std::string mutatedLoop = "bool condition = " + condition + ";\n";
        mutatedLoop += "int value1 = " + value1 + ";\n";
        mutatedLoop += "int value2 = " + value2 + ";\n";
        mutatedLoop += "for (auto x = condition ? value1 : value2 : ";

        size_t colonPos = LoopText.find(':');
        if (colonPos != std::string::npos) {
            mutatedLoop += LoopText.substr(colonPos + 1);
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LoopRange), mutatedLoop);
    }
}

void MutatorFrontendAction_10000::MutatorASTConsumer_10000::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = cxxForRangeStmt().bind("ForLoop");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}