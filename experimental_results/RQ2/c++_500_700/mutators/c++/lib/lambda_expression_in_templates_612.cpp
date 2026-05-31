//source file
#include "../include/Mutator_612.h"

// ========================================================================================================
#define MUT612_OUTPUT 1

void MutatorFrontendAction_612::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
            auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());
            std::string mutatedLambda = "decltype(" + lambdaSource + ".*(&decltype(" + lambdaSource + ")::operator()))";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), mutatedLambda);
        }
    }
}

void MutatorFrontendAction_612::MutatorASTConsumer_612::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = functionDecl(hasDescendant(lambdaExpr().bind("LambdaExpr"))).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}