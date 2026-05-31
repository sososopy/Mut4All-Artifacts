//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Builtin_Arg_With_Invalid_Expr_206
 */ 
class MutatorFrontendAction_206 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(206)

private:
    class MutatorASTConsumer_206 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_206(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Builtin_Arg_With_Invalid_Expr_206.h"

// ========================================================================================================
#define MUT206_OUTPUT 1

void MutatorFrontendAction_206::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *callExpr = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!callExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                           callExpr->getLocation()))
            return;

        unsigned numArgs = callExpr->getNumArgs();
        if (numArgs == 0)
            return;

        unsigned index = getrandom::getRandomIndex(numArgs);
        auto argExpr = callExpr->getArg(index);
        if (!argExpr)
            return;

        SourceRange argRange = argExpr->getSourceRange();
        Rewrite.ReplaceText(argRange, "(invalid_id)");
    }
}

void MutatorFrontendAction_206::MutatorASTConsumer_206::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = callExpr(callee(builtinFunctionDecl())).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}