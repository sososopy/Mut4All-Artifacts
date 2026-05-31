//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_atomic_type_comparisons_42
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
        std::map<const VarDecl*, std::string> atomicVarToNonAtomicVar;
    };
};

//source file
#include "../include/modify_atomic_type_comparisons_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("atomicComparison")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(BO->getExprLoc()))
            return;

        const Expr *lhs = BO->getLHS()->IgnoreParenImpCasts();
        const Expr *rhs = BO->getRHS()->IgnoreParenImpCasts();

        const VarDecl *atomicVarDecl = nullptr;
        const Expr *otherExpr = nullptr;

        if (auto *DRE = dyn_cast<DeclRefExpr>(lhs)) {
            if (DRE->getDecl()->getType()->isAtomicType()) {
                atomicVarDecl = dyn_cast<VarDecl>(DRE->getDecl());
                otherExpr = rhs;
            }
        } else if (auto *DRE = dyn_cast<DeclRefExpr>(rhs)) {
            if (DRE->getDecl()->getType()->isAtomicType()) {
                atomicVarDecl = dyn_cast<VarDecl>(DRE->getDecl());
                otherExpr = lhs;
            }
        }

        if (!atomicVarDecl || !otherExpr)
            return;

        std::string nonAtomicVarName;
        if (atomicVarToNonAtomicVar.find(atomicVarDecl) == atomicVarToNonAtomicVar.end()) {
            nonAtomicVarName = atomicVarDecl->getNameAsString() + "_nonAtomic";
            std::string initStmt = atomicVarDecl->getType().getUnqualifiedType().getAsString() + " " + nonAtomicVarName + " = " + atomicVarDecl->getNameAsString() + ".load();\n";
            Rewrite.InsertTextBefore(BO->getBeginLoc(), initStmt);
            atomicVarToNonAtomicVar[atomicVarDecl] = nonAtomicVarName;
        } else {
            nonAtomicVarName = atomicVarToNonAtomicVar[atomicVarDecl];
        }

        std::string mutatedExpr = (lhs == otherExpr ? nonAtomicVarName : otherExpr->getStmtClassName()) + " " + BO->getOpcodeStr().str() + " " + (rhs == otherExpr ? nonAtomicVarName : otherExpr->getStmtClassName());
        Rewrite.ReplaceText(BO->getSourceRange(), mutatedExpr);
    }
}
  
void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = binaryOperator(hasOperatorName("=="), hasEitherOperand(expr(hasType(isAtomicType())))).bind("atomicComparison");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}