//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Requires_Clause_In_Nested_Lambda_With_Pack_Expansion_117
 */ 
class MutatorFrontendAction_117 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(117)
private:
    class MutatorASTConsumer_117 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_117(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Insert_Requires_Clause_In_Nested_Lambda_With_Pack_Expansion_117.h"

// ========================================================================================================
#define MUT117_OUTPUT 1

void MutatorFrontendAction_117::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *outerLambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("outerLambda")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(outerLambda->getBeginLoc()))
            return;

        const auto *outerCallOp = outerLambda->getCallOperator();
        if (!outerCallOp || outerCallOp->param_size() == 0)
            return;
        const ParmVarDecl *outerParam = outerCallOp->getParamDecl(0);
        std::string outerParamName = outerParam->getNameAsString();

        const ReturnStmt *returnStmt = nullptr;
        const LambdaExpr *innerLambda = nullptr;
        for (const Stmt *stmt : outerLambda->getBody()->children()) {
            if (const auto *rs = dyn_cast<ReturnStmt>(stmt)) {
                if (const auto *innerLambdaTemp = dyn_cast<LambdaExpr>(rs->getRetValue())) {
                    returnStmt = rs;
                    innerLambda = innerLambdaTemp;
                    break;
                }
            }
        }
        if (!returnStmt || !innerLambda)
            return;

        const auto *innerCallOp = innerLambda->getCallOperator();
        if (!innerCallOp)
            return;

        SourceLocation paramListStart = innerCallOp->getParamDeclListStartLoc();
        SourceLocation paramListEnd = innerCallOp->getParamDeclListEndLoc();
        if (paramListStart.isInvalid() || paramListEnd.isInvalid())
            return;

        Rewrite.ReplaceText(SourceRange(paramListStart, paramListEnd), "(auto... params)");

        SourceLocation insertLoc = paramListEnd;
        std::string requiresClause = " requires requires { " + outerParamName + ".size(params...); }";
        Rewrite.InsertText(insertLoc, requiresClause, true, true);
    }
}
  
void MutatorFrontendAction_117::MutatorASTConsumer_117::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = lambdaExpr(
        hasDescendant(
            returnStmt(
                hasReturnValue(
                    lambdaExpr().bind("innerLambda")
                )
            )
        )
    ).bind("outerLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}