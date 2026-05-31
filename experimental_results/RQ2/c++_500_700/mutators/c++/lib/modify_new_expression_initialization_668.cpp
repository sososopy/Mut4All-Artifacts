//source file
#include "../include/modify_new_expression_initialization_668.h"

// ========================================================================================================
#define MUT668_OUTPUT 1

void MutatorFrontendAction_668::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NE = Result.Nodes.getNodeAs<clang::CXXNewExpr>("newExpr")) {
        if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       NE->getBeginLoc()))
            return;

        if (NE->hasInitializer()) {
            auto *initExpr = NE->getInitializer();
            auto initRange = initExpr->getSourceRange();
            auto initText = stringutils::rangetoStr(*(Result.SourceManager), initRange);

            if (initText.find('{') != std::string::npos && initText.find('(') != std::string::npos) {
                std::string mutatedInit = initText;
                mutatedInit.erase(std::remove(mutatedInit.begin(), mutatedInit.end(), '('), mutatedInit.end());
                mutatedInit.erase(std::remove(mutatedInit.begin(), mutatedInit.end(), ')'), mutatedInit.end());

                Rewrite.ReplaceText(CharSourceRange::getTokenRange(initRange), mutatedInit);
            }
        }
    }
}

void MutatorFrontendAction_668::MutatorASTConsumer_668::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxNewExpr(has(cxxConstructExpr(has(initListExpr()))))
                   .bind("newExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}