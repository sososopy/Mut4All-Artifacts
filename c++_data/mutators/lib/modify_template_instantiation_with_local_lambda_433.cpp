//source file
#include "../include/Modify_Template_Instantiation_With_Local_Lambda_433.h"

// ========================================================================================================
#define MUT433_OUTPUT 1

void MutatorFrontendAction_433::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto lambdaSourceRange = LambdaExpr->getSourceRange();
        auto lambdaSourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(lambdaSourceRange),
                                                     *Result.SourceManager, Result.Context->getLangOpts()).str();

        if (lambdaSourceText.find("[]") != std::string::npos) {
            std::string mutatedLambda = lambdaSourceText;
            size_t capturePos = mutatedLambda.find("[]");
            mutatedLambda.replace(capturePos, 2, "[=]");

            Rewrite.ReplaceText(lambdaSourceRange, mutatedLambda);
        }
    }
}

void MutatorFrontendAction_433::MutatorASTConsumer_433::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}