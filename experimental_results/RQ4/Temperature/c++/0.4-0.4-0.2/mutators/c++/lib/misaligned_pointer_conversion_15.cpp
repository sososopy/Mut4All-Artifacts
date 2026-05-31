//source file
#include "../include/misaligned_pointer_conversion_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("builtinAssumeAligned")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                CallExpr->getBeginLoc()))
            return;

        if (CallExpr->getNumArgs() < 1)
            return;

        auto Arg = CallExpr->getArg(0);
        if (!Arg->getType()->isPointerType())
            return;

        std::string intVarName = "misalignedInt";
        std::string intDeclaration = "int " + intVarName + " = 42; /*mut15*/\n";
        Rewrite.InsertTextBefore(CallExpr->getBeginLoc(), intDeclaration);
        Rewrite.ReplaceText(Arg->getSourceRange(), intVarName);
    }
}

void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasName("__builtin_assume_aligned")))).bind("builtinAssumeAligned");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}