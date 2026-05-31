//header file
#pragma once
#include "Mutator_base.h"

/**
 * atomic_type_mismatch_in_comparisons_42
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
    };
};

//source file
#include "../include/atomic_type_mismatch_in_comparisons_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *IF = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
        if (!IF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IF->getIfLoc()))
            return;

        auto *BO = llvm::dyn_cast<clang::BinaryOperator>(IF->getCond());
        if (!BO || BO->getOpcode() != clang::BO_EQ)
            return;

        auto *LHS = BO->getLHS()->IgnoreParenImpCasts();
        auto *RHS = BO->getRHS()->IgnoreParenImpCasts();

        auto *AtomicType = Result.Nodes.getNodeAs<clang::VarDecl>("atomicVar");
        auto *OtherType = Result.Nodes.getNodeAs<clang::VarDecl>("otherVar");

        if (!AtomicType || !OtherType)
            return;

        std::string atomicVarName = AtomicType->getNameAsString();
        std::string otherVarName = OtherType->getNameAsString();

        std::string replacement = "if (" + atomicVarName + " == intVar)";
        Rewrite.ReplaceText(IF->getCond()->getSourceRange(), replacement);
    }
}

void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto atomicMatcher = varDecl(hasType(hasDeclaration(namedDecl(hasName("std::atomic"))))).bind("atomicVar");
    auto otherMatcher = varDecl(unless(hasType(hasDeclaration(namedDecl(hasName("std::atomic")))))).bind("otherVar");
    auto ifStmtMatcher = ifStmt(hasCondition(binaryOperator(hasLHS(expr().bind("lhs")), hasRHS(expr().bind("rhs")), hasOperatorName("==")))).bind("ifStmt");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(atomicMatcher, &callback);
    matchFinder.addMatcher(otherMatcher, &callback);
    matchFinder.addMatcher(ifStmtMatcher, &callback);
    matchFinder.matchAST(Context);
}