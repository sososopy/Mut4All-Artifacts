//source file
#include "../include/modify_builtin_function_call_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                CallExpr->getBeginLoc()))
            return;

        auto callee = CallExpr->getDirectCallee();
        if (callee && callee->getName().startswith("__builtin_")) {
            std::string modifiedCall = Lexer::getSourceText(
                CharSourceRange::getTokenRange(CallExpr->getSourceRange()), 
                Result.Context->getSourceManager(), 
                Result.Context->getLangOpts()).str();

            // Perform mutation by modifying the number of arguments
            if (CallExpr->getNumArgs() > 1) {
                // Decrease the number of arguments
                modifiedCall = modifiedCall.substr(0, modifiedCall.find_last_of(','));
            } else {
                // Increase the number of arguments
                modifiedCall.insert(modifiedCall.find_last_of(')'), ", -1");
            }

            Rewrite.ReplaceText(CallExpr->getSourceRange(), modifiedCall);
        }
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = callExpr(callee(functionDecl(hasName("__builtin_")))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}