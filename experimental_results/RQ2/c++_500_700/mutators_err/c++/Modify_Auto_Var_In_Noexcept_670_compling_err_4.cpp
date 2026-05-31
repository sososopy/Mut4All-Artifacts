//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_auto_var_in_noexcept_670
 */ 
class MutatorFrontendAction_670 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(670)

private:
    class MutatorASTConsumer_670 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_670(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_auto_var_in_noexcept_670.h"

// ========================================================================================================
#define MUT670_OUTPUT 1

void MutatorFrontendAction_670::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                LambdaExpr->getBeginLoc()))
            return;

        auto *CallOp = LambdaExpr->getCallOperator();
        if (!CallOp || !CallOp->getType()->isNoThrow())
            return;

        if (auto *NoexceptSpec = CallOp->getNoThrowExpr()) {
            if (auto *DeclRef = dyn_cast<DeclRefExpr>(NoexceptSpec->IgnoreImplicit())) {
                if (isa<VarDecl>(DeclRef->getDecl())) {
                    Rewrite.ReplaceText(NoexceptSpec->getSourceRange(), "true");
                }
            }
        }
    }
}

void MutatorFrontendAction_670::MutatorASTConsumer_670::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(has(cxxMethodDecl(hasNoThrowExpr(expr(hasDescendant(declRefExpr(to(varDecl().bind("autoVar")))))
    )))).bind("lambdaExpr");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}