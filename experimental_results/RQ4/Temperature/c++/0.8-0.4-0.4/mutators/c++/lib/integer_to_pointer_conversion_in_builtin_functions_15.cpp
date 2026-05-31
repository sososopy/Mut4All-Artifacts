//source file
#include "../include/integer_to_pointer_conversion_in_builtin_functions_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                         CallExpr->getBeginLoc()))
            return;

        auto *FD = CallExpr->getDirectCallee();
        if (!FD || !FD->getBuiltinID())
            return;

        for (unsigned i = 0; i < CallExpr->getNumArgs(); ++i) {
            auto *Arg = CallExpr->getArg(i);
            if (Arg->getType()->isPointerType()) {
                std::string replacement = "42"; // Replace with an integer literal
                Rewrite.ReplaceText(Arg->getSourceRange(), replacement);
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasAttr(clang::attr::Builtin)))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}