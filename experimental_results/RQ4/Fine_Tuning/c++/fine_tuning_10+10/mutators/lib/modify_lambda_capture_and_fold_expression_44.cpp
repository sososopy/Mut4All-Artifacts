//source file
#include "../include/Modify_Lambda_Capture_And_Fold_Expression_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto &SM = Result.Context->getSourceManager();
        auto &LangOpts = Result.Context->getLangOpts();
        
        // Get the lambda source code
        auto lambdaSourceRange = LambdaExpr->getSourceRange();
        auto lambdaSourceText = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(lambdaSourceRange), SM, LangOpts);

        // Convert llvm::StringRef to std::string
        std::string lambdaSourceTextStr = lambdaSourceText.str();

        // Check if a suitable variable exists in the surrounding scope
        bool hasSuitableVariable = false;
        for (auto &capture : LambdaExpr->captures()) {
            if (capture.capturesVariable()) {
                hasSuitableVariable = true;
                break;
            }
        }

        // If no suitable variable exists, declare a new integer variable in the surrounding scope
        if (!hasSuitableVariable) {
            Rewrite.InsertText(LambdaExpr->getBeginLoc(), newVariable + "\n");
        }

        // Modify the lambda capture list to include 'x'
        std::string modifiedLambdaSourceText = lambdaSourceTextStr;
        size_t captureListStart = modifiedLambdaSourceText.find('[');
        size_t captureListEnd = modifiedLambdaSourceText.find(']', captureListStart);
        if (captureListStart != std::string::npos && captureListEnd != std::string::npos) {
            modifiedLambdaSourceText.insert(captureListEnd, "x, ");
        }

        // Modify the fold expression within the lambda
        size_t foldExprStart = modifiedLambdaSourceText.find("([");
        size_t foldExprEnd = modifiedLambdaSourceText.find(") + ...", foldExprStart);
        if (foldExprStart != std::string::npos && foldExprEnd != std::string::npos) {
            modifiedLambdaSourceText.replace(foldExprStart, foldExprEnd - foldExprStart, "(([");
        }

        // Replace the original lambda with the modified one
        Rewrite.ReplaceText(lambdaSourceRange, modifiedLambdaSourceText);
    }
}

void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target lambda expression for mutation
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}