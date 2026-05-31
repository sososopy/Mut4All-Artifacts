//source file
#include "../include/modify_lambda_in_immediate_function_call_618.h"

// ========================================================================================================
#define MUT618_OUTPUT 1

void MutatorFrontendAction_618::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("ImmediateFunctionCall")) {
        //Filter nodes in header files
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getBeginLoc()))
            return;

        for (unsigned i = 0; i < CallExpr->getNumArgs(); ++i) {
            if (auto *LambdaExpr = dyn_cast<clang::LambdaExpr>(CallExpr->getArg(i))) {
                //Get the source code text of target node
                auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LambdaExpr->getSourceRange());

                //Perform mutation on the source code text by modifying the lambda's capture list and return type
                std::string mutatedLambda = lambdaSource;
                if (lambdaSource.find("[]") != std::string::npos) {
                    mutatedLambda.replace(lambdaSource.find("[]"), 2, "[x = 0]");
                }
                if (lambdaSource.find("->") != std::string::npos) {
                    mutatedLambda.replace(lambdaSource.find("->"), 2, "mutable ->");
                } else {
                    mutatedLambda.insert(lambdaSource.find("{"), "mutable ");
                }

                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaExpr->getSourceRange()), mutatedLambda);
            }
        }
    }
}

void MutatorFrontendAction_618::MutatorASTConsumer_618::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(hasAnyArgument(lambdaExpr())).bind("ImmediateFunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}