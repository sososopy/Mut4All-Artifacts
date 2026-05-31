//source file
#include "../include/modify_lambda_capture_and_type_usage_588.h"

// ========================================================================================================
#define MUT588_OUTPUT 1

void MutatorFrontendAction_588::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto &SM = Rewrite.getSourceMgr();
        auto &LangOpts = Result.Context->getLangOpts();
        auto CaptureList = LambdaExpr->getCaptureDefault();
        std::string NewCaptureList;
        
        if (CaptureList == clang::LCD_ByCopy) {
            NewCaptureList = "[&]";
        } else if (CaptureList == clang::LCD_ByRef) {
            NewCaptureList = "[=]";
        }

        auto LambdaRange = clang::CharSourceRange::getTokenRange(
            LambdaExpr->getSourceRange());
        std::string LambdaText = clang::Lexer::getSourceText(
            LambdaRange, SM, LangOpts).str();

        size_t CapturePos = LambdaText.find('[');
        size_t CaptureEndPos = LambdaText.find(']');
        if (CapturePos != std::string::npos && CaptureEndPos != std::string::npos) {
            LambdaText.replace(CapturePos, CaptureEndPos - CapturePos + 1, NewCaptureList);
        }

        if (auto *DeclType = Result.Nodes.getNodeAs<clang::DecltypeType>("Decltype")) {
            std::string TypeStr = DeclType->getUnderlyingType().getAsString();
            size_t DecltypePos = LambdaText.find("decltype");
            if (DecltypePos != std::string::npos) {
                LambdaText.replace(DecltypePos, 8, TypeStr);
            }
        }

        Rewrite.ReplaceText(LambdaRange, LambdaText);
    }
}

void MutatorFrontendAction_588::MutatorASTConsumer_588::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr().bind("Lambda");
    auto decltypeMatcher = decltypeType().bind("Decltype");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(decltypeMatcher, &callback);
    matchFinder.matchAST(Context);
}