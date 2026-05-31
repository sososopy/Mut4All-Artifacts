//header file
#pragma once
#include "Mutator_base.h"
#include <unordered_map>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_atomic_shift_operands_676
 */ 
class MutatorFrontendAction_676 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(676)

private:
    class MutatorASTConsumer_676 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_676(Rewriter &R) : TheRewriter(R) {}
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
        std::unordered_map<const clang::VarDecl*, std::string> atomicToRegularVarMap;
    };
};

//source file
#include "../include/modify_atomic_shift_operands_676.h"

// ========================================================================================================
#define MUT676_OUTPUT 1

void MutatorFrontendAction_676::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("shiftOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(BO->getExprLoc()))
            return;

        const Expr *LHS = BO->getLHS()->IgnoreParenImpCasts();
        if (const auto *DRE = dyn_cast<DeclRefExpr>(LHS)) {
            if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
                if (VD->getType()->isAtomicType()) {
                    std::string regularVarName;
                    if (atomicToRegularVarMap.find(VD) != atomicToRegularVarMap.end()) {
                        regularVarName = atomicToRegularVarMap[VD];
                    } else {
                        regularVarName = "regularVar_" + VD->getNameAsString();
                        std::string typeStr = VD->getType().getUnqualifiedType().getAsString();
                        std::string declStr = typeStr + " " + regularVarName + " = 0;\n";
                        Rewrite.InsertTextBefore(BO->getExprLoc(), declStr);
                        atomicToRegularVarMap[VD] = regularVarName;
                    }
                    std::string newExpr = regularVarName + " " + BO->getOpcodeStr().str() + " " + Rewrite.getRewrittenText(BO->getRHS()->getSourceRange());
                    Rewrite.ReplaceText(BO->getSourceRange(), newExpr);
                }
            }
        }
    }
}

void MutatorFrontendAction_676::MutatorASTConsumer_676::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher shiftMatcher = binaryOperator(hasOperatorName("<<"),
                                                   hasLHS(expr(hasType(isAtomic())))).bind("shiftOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(shiftMatcher, &callback);
    matchFinder.matchAST(Context);
}