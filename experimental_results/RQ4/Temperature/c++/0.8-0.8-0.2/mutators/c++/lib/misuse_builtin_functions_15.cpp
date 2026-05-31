//source file
#include "../include/misuse_builtin_functions_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const CallExpr *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getExprLoc()))
            return;

        const FunctionDecl *FD = CE->getDirectCallee();
        if (!FD)
            return;

        bool expectsPointer = false;
        for (const auto *Param : FD->parameters()) {
            if (Param->getType()->isPointerType()) {
                expectsPointer = true;
                break;
            }
        }

        if (!expectsPointer)
            return;

        for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
            if (CE->getArg(i)->getType()->isPointerType()) {
                auto *Arg = CE->getArg(i);
                std::string replacement = "42"; // Use an integer as a replacement
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(Arg->getSourceRange()), replacement);
            }
        }
    }
}

void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl()), hasAnyArgument(expr().bind("arg"))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}