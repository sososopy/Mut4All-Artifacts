//source file
#include "../include/modify_variadic_template_argument_expansion_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("variadicExpansion")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getExprLoc()))
            return;

        auto sourceRange = CE->getSourceRange();
        auto originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

        std::string mutatedText = "([](auto... args) { (" + originalText + ", ...); }(args...))";
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}

void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(hasDescendant(exprWithCleanups(hasDescendant(unaryExprOrTypeTraitExpr())))).bind("variadicExpansion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}