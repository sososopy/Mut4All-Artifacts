//source file
#include "../include/Add_Explicit_Template_Args_To_Constexpr_Call_205.h"

// ========================================================================================================
#define MUT205_OUTPUT 1

void MutatorFrontendAction_205::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        const FunctionDecl *FD = CE->getDirectCallee();
        if (!FD || FD->getTemplateSpecializationKind() != clang::TSK_Undeclared)
            return;

        auto *CXXCE = dyn_cast<clang::CXXMemberCallExpr>(CE);
        if (!CXXCE)
            return;

        // Check if the call already has explicit template arguments
        const Expr *Callee = CE->getCallee();
        if (const auto *ME = dyn_cast<clang::MemberExpr>(Callee)) {
            if (ME->hasTemplateKeyword()) {
                return;
            }
        }

        SourceLocation insertLoc = Callee->getEndLoc();
        if (insertLoc.isInvalid())
            return;

        Rewrite.InsertText(insertLoc, "<1>", true, true);
    }
}

void MutatorFrontendAction_205::MutatorASTConsumer_205::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(
        callee(functionDecl(isConstexpr()))
    ).bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}