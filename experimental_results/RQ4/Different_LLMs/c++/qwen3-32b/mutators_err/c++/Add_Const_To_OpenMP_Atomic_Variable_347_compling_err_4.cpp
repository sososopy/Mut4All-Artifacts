//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Const_To_OpenMP_Atomic_Variable_347
 */ 
class MutatorFrontendAction_347 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(347)

private:
    class MutatorASTConsumer_347 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_347(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Const_To_OpenMP_Atomic_Variable_347.h"

// ========================================================================================================
#define MUT347_OUTPUT 1

void MutatorFrontendAction_347::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *atomicStmt = Result.Nodes.getNodeAs<clang::OMPAtomicStmt>("atomicStmt")) {
        if (!atomicStmt || !Result.Context->getSourceManager().isWrittenInMainFile(atomicStmt->getBeginLoc()))
            return;

        auto *capturedStmt = atomicStmt->getCapturedStmt();
        if (auto *compoundStmt = dyn_cast<clang::CompoundStmt>(capturedStmt)) {
            for (const auto *stmt : compoundStmt->body()) {
                if (auto *binaryOp = dyn_cast<clang::BinaryOperator>(stmt)) {
                    if (binaryOp->isAssignmentOp()) {
                        auto *lhs = binaryOp->getLHS()->IgnoreImplicit();
                        if (auto *declRefExpr = dyn_cast<clang::DeclRefExpr>(lhs)) {
                            auto *varDecl = dyn_cast<clang::VarDecl>(declRefExpr->getDecl());
                            if (varDecl && !varDecl->getType().isConstQualified()) {
                                auto typeRange = varDecl->getTypeSourceInfo()->getTypeLoc().getSourceRange();
                                Rewrite.InsertTextBefore(typeRange, "const ");
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_347::MutatorASTConsumer_347::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::openmp::ompAtomicStmt().bind("atomicStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}