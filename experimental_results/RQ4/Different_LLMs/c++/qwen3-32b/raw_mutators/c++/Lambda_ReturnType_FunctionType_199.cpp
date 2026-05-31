//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Expr.h"

/**
 * Lambda_ReturnType_FunctionType_199
 */ 
class MutatorFrontendAction_199 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(199)

private:
    class MutatorASTConsumer_199 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_199(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Lambda_ReturnType_FunctionType_199.h"

// ========================================================================================================
#define MUT199_OUTPUT 1

void MutatorFrontendAction_199::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;
        if (!lambda->hasTrailingReturn())
            return;
        auto *callOp = lambda->getCallOperator();
        if (!callOp || callOp->param_size() < 1)
            return;
        auto *firstParam = callOp->getParamDecl(0);
        if (!firstParam)
            return;
        SourceRange returnRange = lambda->getTrailingReturnTypeSourceRange();
        if (returnRange.isInvalid())
            return;
        ASTContext &Context = *Result.Context;
        PrintingPolicy policy = Context.getPrintingPolicy();
        std::string paramTypeStr = firstParam->getType().getAsString(policy);
        std::string paramNameStr = firstParam->getNameAsString();
        std::string paramStr = " (" + paramTypeStr + " " + paramNameStr + ")";
        Rewrite.InsertTextAfter(returnRange.getEnd(), paramStr);
    }
}

void MutatorFrontendAction_199::MutatorASTConsumer_199::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(
        hasTrailingReturn(true),
        hasCallOperator(
            cxxMethodDecl(
                hasParameterCount(1, greaterThan(0))
            )
        )
    ).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}