//header file
#pragma once
#include "Mutator_base.h"

/**
 * atomic_type_mismatch_in_conditional_42
 */ 
class MutatorFrontendAction_42 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(42)

private:
    class MutatorASTConsumer_42 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_42(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const VarDecl *, std::string> atomicVars;
    };
};

//source file
#include "../include/atomic_type_mismatch_in_conditional_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Cond = Result.Nodes.getNodeAs<clang::IfStmt>("conditional")) {
        if (!Cond || !Result.Context->getSourceManager().isWrittenInMainFile(
                         Cond->getBeginLoc()))
            return;

        auto *BinOp = dyn_cast<BinaryOperator>(Cond->getCond());
        if (!BinOp || !BinOp->isComparisonOp())
            return;

        Expr *lhs = BinOp->getLHS()->IgnoreParenImpCasts();
        Expr *rhs = BinOp->getRHS()->IgnoreParenImpCasts();

        const VarDecl *lhsVar = nullptr;
        const VarDecl *rhsVar = nullptr;

        if (auto *DRE = dyn_cast<DeclRefExpr>(lhs))
            lhsVar = dyn_cast<VarDecl>(DRE->getDecl());
        if (auto *DRE = dyn_cast<DeclRefExpr>(rhs))
            rhsVar = dyn_cast<VarDecl>(DRE->getDecl());

        if (!lhsVar && !rhsVar)
            return;

        const VarDecl *atomicCandidate = lhsVar ? lhsVar : rhsVar;
        if (atomicVars.find(atomicCandidate) == atomicVars.end()) {
            std::string atomicType = "std::atomic<" + atomicCandidate->getType().getAsString() + ">";
            atomicVars[atomicCandidate] = atomicType;

            std::string atomicDecl = atomicType + " " + atomicCandidate->getNameAsString() + " = 0;\n";
            Rewrite.InsertTextBefore(Cond->getBeginLoc(), atomicDecl);
        }

        std::string replacement = "/*mut42*/" + atomicCandidate->getNameAsString();
        if (lhsVar == atomicCandidate)
            Rewrite.ReplaceText(lhs->getSourceRange(), replacement);
        else if (rhsVar == atomicCandidate)
            Rewrite.ReplaceText(rhs->getSourceRange(), replacement);
    }
}

void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = ifStmt(hasCondition(binaryOperator(isComparisonOp()).bind("binaryOp"))).bind("conditional");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}