//source file
#include "../include/Add_Constexpr_Loop_Container_Modifier_462.h"

// ========================================================================================================
#define MUT462_OUTPUT 1

void MutatorFrontendAction_462::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callToModify")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                           CE->getBeginLoc()))
            return;

        int loopCount = 10 + getrandom::getRandomIndex(91);
        std::string loopCode = "for (int i = 0; i < " + std::to_string(loopCount) + "; ++i)\n    ";
        loopCode += Rewrite.getRewrittenText(CharSourceRange::getTokenRange(CE->getSourceRange()));

        Rewrite.ReplaceText(CE->getSourceRange(), loopCode);
    }
}

void MutatorFrontendAction_462::MutatorASTConsumer_462::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(
        hasAncestor(functionDecl(isConstexpr()))
    ).bind("callToModify");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}