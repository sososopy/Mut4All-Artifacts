//source file
#include "../include/replace_function_with_recursive_call_381.h"

// ========================================================================================================
#define MUT381_OUTPUT 1

void MutatorFrontendAction_381::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (!FD->hasBody() || FD->isMain())
            return;
        
        // Record the function for potential mutation
        targetFunctions.push_back(FD);
    } else if (auto *CS = Result.Nodes.getNodeAs<clang::CallExpr>("CallSites")) {
        if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CS->getBeginLoc()))
            return;

        for (const auto *FD : targetFunctions) {
            if (CS->getDirectCallee() == FD) {
                // Get the source code text of target node
                auto callText = stringutils::rangetoStr(*(Result.SourceManager), CS->getSourceRange());
                // Perform mutation on the source code text by applying string replacement
                callText = FD->getNameAsString() + "(" + callText + ")";
                // Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(CS->getSourceRange()), callText);
                break;
            }
        }
    }
}

void MutatorFrontendAction_381::MutatorASTConsumer_381::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto functionMatcher = functionDecl(isDefinition()).bind("Functions");
    auto callExprMatcher = callExpr().bind("CallSites");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(callExprMatcher, &callback);
    matchFinder.matchAST(Context);
}