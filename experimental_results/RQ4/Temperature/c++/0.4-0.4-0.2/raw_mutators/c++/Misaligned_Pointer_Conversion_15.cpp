//header file
#pragma once
#include "Mutator_base.h"

/**
 * misaligned_pointer_conversion_15
 */ 
class MutatorFrontendAction_15 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(15)

private:
    class MutatorASTConsumer_15 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_15(Rewriter &R) : TheRewriter(R) {}
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