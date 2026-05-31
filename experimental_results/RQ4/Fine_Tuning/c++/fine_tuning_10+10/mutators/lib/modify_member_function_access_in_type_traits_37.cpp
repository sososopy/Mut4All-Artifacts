//source file
#include "../include/modify_member_function_access_in_type_traits_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
            return;

        auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());

        size_t pos = lambdaSource.find("::Func()");
        if (pos != std::string::npos) {
            lambdaSource.replace(pos, 7, "::NonExistentFunc()");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaSource);
        }
    }
}

void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(decltypeType())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}