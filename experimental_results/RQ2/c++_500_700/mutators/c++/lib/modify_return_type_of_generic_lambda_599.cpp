//source file
#include "../include/modify_return_type_of_generic_lambda_599.h"

// ========================================================================================================
#define MUT599_OUTPUT 1

void MutatorFrontendAction_599::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        const auto *LambdaType = LambdaExpr->getLambdaClass();
        if (!LambdaType || LambdaType->isDependentType())
            return;

        const auto *TemplateTypeParm = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParm");
        if (!TemplateTypeParm)
            return;

        std::string TemplateTypeName = TemplateTypeParm->getNameAsString();
        auto LambdaSourceRange = LambdaExpr->getSourceRange();
        std::string LambdaSourceText = stringutils::rangetoStr(*(Result.SourceManager), LambdaSourceRange);

        size_t arrowPos = LambdaSourceText.find("->");
        if (arrowPos != std::string::npos) {
            size_t endPos = LambdaSourceText.find('{', arrowPos);
            if (endPos != std::string::npos) {
                LambdaSourceText.replace(arrowPos + 2, endPos - (arrowPos + 2), TemplateTypeName);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaSourceRange), LambdaSourceText);
            }
        }
    }
}

void MutatorFrontendAction_599::MutatorASTConsumer_599::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr().bind("LambdaExpr");
    auto templateTypeMatcher = templateTypeParmDecl().bind("TemplateTypeParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(templateTypeMatcher, &callback);
    matchFinder.matchAST(Context);
}