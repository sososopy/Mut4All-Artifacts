//source file
#include "../include/nested_lambda_with_init_capture_and_concept_constraints_554.h"

// ========================================================================================================
#define MUT554_OUTPUT 1

void MutatorFrontendAction_554::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LambdaExpr->getSourceRange());

        std::string nestedLambda = "[captured{input}]() requires true { return captured + value; };";
        size_t pos = lambdaSource.find("{");
        if (pos != std::string::npos) {
            lambdaSource.insert(pos + 1, nestedLambda);
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), lambdaSource);
    }
}

void MutatorFrontendAction_554::MutatorASTConsumer_554::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasAncestor(functionTemplateDecl())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}