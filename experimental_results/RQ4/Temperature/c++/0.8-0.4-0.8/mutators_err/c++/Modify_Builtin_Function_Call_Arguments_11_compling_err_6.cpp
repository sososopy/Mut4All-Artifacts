//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_builtin_function_call_arguments_11
 */
class MutatorFrontendAction_11 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(11)

private:
    class MutatorASTConsumer_11 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_11(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/modify_builtin_function_call_arguments_11.h"

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
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasAttr<AttrKind::Builtin>())).bind("builtinCall"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}