//source file
#include "../include/Add_Lambda_To_Template_Call_269.h"

// ========================================================================================================
#define MUT269_OUTPUT 1

void MutatorFrontendAction_269::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("templateCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getBeginLoc()))
            return;

        const FunctionDecl *Callee = CallExpr->getDirectCallee();
        if (!Callee || !Callee->getTemplatedKind() == FunctionDecl::TK_FunctionTemplate)
            return;

        std::string lambdaCode = "[&]() { /* complex logic */ }";
        std::string callText = stringutils::rangetoStr(*(Result.SourceManager), CallExpr->getSourceRange());

        size_t insertPos = callText.find_last_of(')');
        if (insertPos != std::string::npos) {
            callText.insert(insertPos, ", " + lambdaCode);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CallExpr->getSourceRange()), callText);
        }
    }
}

void MutatorFrontendAction_269::MutatorASTConsumer_269::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasDescendant(lambdaExpr())))).bind("templateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}