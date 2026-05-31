//source file
#include "../include/Mutator_314.h"

// ========================================================================================================
#define MUT314_OUTPUT 1

void MutatorFrontendAction_314::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        const auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr");
        if (!CallExpr)
            return;

        // Get the source code text of target node
        auto callRange = CallExpr->getSourceRange();
        std::string callText = stringutils::rangetoStr(*(Result.SourceManager), callRange);

        // Perform mutation by modifying the number of arguments in the call expression
        size_t argCount = CallExpr->getNumArgs();
        std::string newArgs = (argCount > 1) ? "(1)" : "(1, 2)";
        std::string mutatedCallText = callText.substr(0, callText.find('(')) + newArgs;

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(callRange), mutatedCallText);
    }
}

void MutatorFrontendAction_314::MutatorASTConsumer_314::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto lambdaMatcher = lambdaExpr(
        hasDescendant(callExpr().bind("callExpr"))
    ).bind("lambdaExpr");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}