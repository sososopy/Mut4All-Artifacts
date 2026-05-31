//source file
#include "../include/modify_lambda_parameter_types_541.h"

// ========================================================================================================
#define MUT541_OUTPUT 1

void MutatorFrontendAction_541::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
            return;

        const auto &CaptureDefault = LE->getCaptureDefault();
        if (CaptureDefault != clang::LCD_None) {
            return;
        }

        for (auto *Child : LE->getBody()->children()) {
            if (auto *NestedLE = dyn_cast<clang::LambdaExpr>(Child)) {
                auto SourceRange = NestedLE->getSourceRange();
                auto StartLoc = SourceRange.getBegin();
                auto EndLoc = SourceRange.getEnd();

                llvm::StringRef OriginalText = clang::Lexer::getSourceText(
                    clang::CharSourceRange::getTokenRange(SourceRange),
                    Result.Context->getSourceManager(),
                    Result.Context->getLangOpts());

                std::string MutatedText = OriginalText.str();
                size_t ParamStart = MutatedText.find('(');
                size_t ParamEnd = MutatedText.find(')', ParamStart);

                if (ParamStart != std::string::npos && ParamEnd != std::string::npos) {
                    std::string Params = MutatedText.substr(ParamStart + 1, ParamEnd - ParamStart - 1);
                    std::string NewParams = "decltype(x)... y";
                    MutatedText.replace(ParamStart + 1, ParamEnd - ParamStart - 1, NewParams);
                    size_t BodyStart = MutatedText.find('{', ParamEnd);
                    if (BodyStart != std::string::npos) {
                        MutatedText.insert(BodyStart, " requires true");
                    }
                }

                Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(SourceRange), MutatedText);
            }
        }
    }
}

void MutatorFrontendAction_541::MutatorASTConsumer_541::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}