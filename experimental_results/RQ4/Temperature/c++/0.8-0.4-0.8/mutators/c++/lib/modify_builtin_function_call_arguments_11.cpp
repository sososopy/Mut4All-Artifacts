//source file
#include "../include/modify_builtin_function_call_arguments_11.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Lex/Lexer.h"

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
            return;

        const FunctionDecl *FD = CE->getDirectCallee();
        if (!FD || !FD->getBuiltinID())
            return;

        std::string mutatedCall = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(CE->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
        size_t pos = mutatedCall.find_last_of(')');
        if (pos != std::string::npos) {
            mutatedCall.insert(pos, ", unknown_var");
        }
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCall);
    }
}

void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasAttr(clang::attr::Builtin)))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}