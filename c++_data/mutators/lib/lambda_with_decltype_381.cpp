//source file
#include "../include/Lambda_With_Decltype_381.h"

// ========================================================================================================
#define MUT381_OUTPUT 1

void MutatorFrontendAction_381::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!Lambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                Lambda->getBeginLoc()))
            return;

        const CXXMethodDecl *MethodDecl = dyn_cast<CXXMethodDecl>(Lambda->getLambdaClass()->getParent());
        if (MethodDecl && MethodDecl->isInstance()) {
            auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), Lambda->getSourceRange());
            size_t arrowPos = lambdaSource.find("->");
            if (arrowPos == std::string::npos) {
                size_t bracePos = lambdaSource.find("{");
                if (bracePos != std::string::npos) {
                    lambdaSource.insert(bracePos, " -> decltype(this)");
                }
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(Lambda->getSourceRange()), lambdaSource);
        }
    }
}

void MutatorFrontendAction_381::MutatorASTConsumer_381::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}